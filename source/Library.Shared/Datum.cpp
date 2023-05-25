#include "pch.h"
#include "Datum.h"
#include "Algorithms.h"
#include "Scope.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    const typename Datum::String Datum::VECTOR_STRING_FORMAT_PREFIX = "vector"s;
    const typename Datum::String Datum::VECTOR_STRING_FORMAT = VECTOR_STRING_FORMAT_PREFIX + "<%f|%f|%f|%f>"s;
    const typename Datum::String Datum::MATRIX_STRING_FORMAT_PREFIX = "matrix"s;
    const typename Datum::String Datum::MATRIX_STRING_FORMAT = MATRIX_STRING_FORMAT_PREFIX + "[<%f|%f|%f|%f>,<%f|%f|%f|%f>,<%f|%f|%f|%f>,<%f|%f|%f|%f>]"s;

    const typename Datum::String Datum::TO_STRING_DATUM_TYPE[] = { // Must match the definition of DatumType, in Datum.h
        "unknown"s,
        "integer"s,
        "float"s,
        "string"s,
        "vector"s,
        "matrix"s,
        "pointer"s,
        "table"s,
        "internalTable"s,
        "externalTable"s
    };

    const typename Datum::size_type Datum::TYPE_SIZES[] = { // Must match the definition of DatumType, in Datum.h
        size_type(0), // Unknown
        sizeof(Integer),
        sizeof(Float),
        sizeof(String),
        sizeof(Vector),
        sizeof(Matrix),
        sizeof(Pointer),
        size_type(0), // Table, never actually used within the datum
        sizeof(InternalTablePointer),
        sizeof(ExternalTablePointer)
    };

    Datum::Datum(
        InternalTablePointer scalar,
        ExternalTablePointer parent,
        GrowCapacityFunctorType growCapacityFunctor
    ) : Datum{DatumType::InternalTable, growCapacityFunctor} {
        _parent = parent;
        scalar->_parent = parent;
        _data.t = new InternalTablePointer{std::forward<InternalTablePointer>(scalar)};
    }

    Datum::Datum(const Datum& other) : _size{other._size}, _type{other._type}, _isDataInternal{other._isDataInternal}, _isDataExternalConst{other._isDataExternalConst} {
        if (other._isDataInternal) {
            assert(other.ActualType() != DatumType::ExternalTable);
            Reserve(other._capacity);

            if (other.ActualType() == DatumType::String) {
                for (auto i = size_type(0); i < _size; ++i) {
                    new (_data.s + i) String{*(other._data.s + i)};
                }
            } else if (other.ActualType() == DatumType::InternalTable) {
                for (auto i = size_type(0); i < _size; ++i) {
                    new (_data.t + i) InternalTablePointer{(*(other._data.t + i))->Clone()};
                }
            } else {
                assert(other.ActualType() != DatumType::Table);
                std::memcpy(_data.vp, other._data.vp, _size * TypeSize());
            }

            PromulgateParent();
        } else {
            _capacity = other._capacity;
            _data.vp = other._data.vp;
        }
    }

    Datum::Datum(Datum&& other) noexcept
        : _size{other._size}
        , _capacity{other._capacity}
        , _type{other._type}
        , _isDataInternal{other._isDataInternal}
        , _growCapacityFunctor{other._growCapacityFunctor}
    {
        _data.vp = other._data.vp;
        assert(other.ActualType() != DatumType::Table);
        PromulgateParent();
        other._data.vp = nullptr;
        other._size = size_type(0);
        other._capacity = size_type(0);
        other._type = DatumType::Unknown;
        other._isDataInternal = true;
        other._isDataExternalConst = false;
    }

    Datum& Datum::operator=(const Datum& other) {
        Datum copy{other};
        swap(copy);
        return *this;
    }

    Datum& Datum::operator=(Datum&& other) noexcept {
        if (this != &other) {
            if (_isDataInternal) {
                Clear();
                ShrinkToFit();
            }

            _data.vp = other._data.vp;
            _size = other._size;
            _capacity = other._capacity;
            _type = other._type;
            _isDataInternal = other._isDataInternal;
            _isDataExternalConst = other._isDataExternalConst;

            assert(other.ActualType() != DatumType::Table);
            if (other.ActualType() == DatumType::InternalTable) {
                PromulgateParent();
            }

            other._data.vp = nullptr;
            other._size = size_type(0);
            other._capacity = size_type(0);
            other._type = DatumType::Unknown;
            other._isDataInternal = true;
            other._isDataExternalConst = false;
        }

        return *this;
    }

    Datum& Datum::operator=(InternalTablePointer scalar) {
        scalar->_parent = _parent;
        return AssignFromScalarForward(DatumType::InternalTable, std::forward<InternalTablePointer>(scalar));
    }

    Datum& Datum::operator=(Table&& scalar) {
        return operator=(std::forward<InternalTablePointer>(std::make_unique<Table>(std::forward<Table>(scalar))));
    }

    Datum& Datum::operator=(const Table& scalar) {
        return operator=(std::forward<InternalTablePointer>(scalar.Clone()));
    }

    Datum::~Datum() {
        if (_isDataInternal) {
            Clear();
            ShrinkToFit();
        }
    }

    Datum::Datum(Pointer scalar, GrowCapacityFunctorType growCapacityFunctor)
        : Datum{DatumType::Pointer, growCapacityFunctor}
    {
        _data.vp = malloc(TypeSize());
        assert(_data.vp != nullptr);
        *(_data.p) = scalar;
    }

    void Datum::BeginAssignFromScalar(DatumType type) {
        assert((type != DatumType::Table) && (type != DatumType::ExternalTable));

        if (_isDataInternal) {
            Clear();
            ShrinkToFit();
        }

        _type = type;
        _size = size_type(1);
        _capacity = size_type(1);
        _isDataInternal = true;
        _isDataExternalConst = false;
    }

    Datum& Datum::operator=(Pointer scalar) {
        BeginAssignFromScalar(DatumType::Pointer);

        _data.vp = malloc(TypeSize());
        assert(_data.vp != nullptr);
        *(_data.p) = scalar;

        return *this;
    }

    bool Datum::operator==(const Datum& other) const {
        if ((_type != other._type) || (_size != other._size)) {
            return false;
        }

        if (_data.vp == other._data.vp) {
            return true;
        }

        switch (_type) {

        case DatumType::String:
            {
                for (auto i = size_type(0); i < _size; ++i) {
                    if ((*(_data.s + i)) != (*(other._data.s + i))) {
                        return false;
                    }
                }
            }
            break;

        case DatumType::Pointer:
            {
                for (auto i = size_type(0); i < _size; ++i) {
                    if ((*(_data.p + i) != *(other._data.p + i))
                        && (*(_data.p + i) != nullptr)
                        && !((*(_data.p + i))->Equals(*(other._data.p + i)))
                    ) {
                        return false;
                    }
                }
            }
            break;

        case DatumType::InternalTable:
            {
                for (auto i = size_type(0); i < _size; ++i) {
                    if ((*(_data.t + i) != *(other._data.t + i))
                        && ((_data.t + i)->operator bool())
                        && !((*(_data.t + i))->Equals((other._data.t + i)->get()))
                    ) {
                        return false;
                    }
                }
            }
            break;

        case DatumType::ExternalTable:
            {
                for (auto i = size_type(0); i < _size; ++i) {
                    if ((*(_data.x + i) != *(other._data.x + i))
                        && (*(_data.x + i) != nullptr)
                        && !((*(_data.x + i))->Equals(*(other._data.x + i)))
                    ) {
                        return false;
                    }
                }
            }
            break;

        default:
            return (_type == DatumType::Unknown) || (memcmp(_data.vp, other._data.vp, _size * TypeSize()) == 0);

        }

        return true;
    }

    bool Datum::operator==(const Pointer scalar) const {
        if (scalar == nullptr) {
            return _data.vp == nullptr;
        }

        if ((_type != DatumType::Pointer) || (_size != size_type(1))) {
            return false;
        }

        return IsElementEqualToPointer(scalar, size_type(0));
    }

    bool Datum::operator==(const InternalTablePointer& scalar) const {
        if ((ApparentType() != DatumType::Table) || (_size != size_type(1))) {
            return false;
        }

        return IsElementEqualToTable(scalar.get(), size_type(0));
    }

    bool Datum::operator==(const Table& scalar) const {
        if ((ApparentType() != DatumType::Table) || (_size != size_type(1))) {
            return false;
        }

        return IsElementEqualToTable(&scalar, size_type(0));
    }

    bool Datum::IsElementEqualToPointer(Pointer pointer, size_type index) const {
        bool isPointerSame = *(_data.p + index) == pointer;
        bool isDataNotNull = *(_data.p + index) != nullptr;

        return isPointerSame || (isDataNotNull && (*(_data.p + index))->Equals(pointer));
    }

    bool Datum::IsElementEqualToTable(const Table* table, size_type index) const {
        assert(_type != DatumType::Table);
        return (_type == DatumType::InternalTable) ? IsElementEqualToInternalTable(table, index) : IsElementEqualToExternalTable(table, index);
    }

    inline bool Datum::IsElementEqualToInternalTable(const Table* table, size_type index) const {
        return ((*(_data.t + index)).get() == table) || (*(_data.t + index))->Equals(table);
    }

    inline bool Datum::IsElementEqualToExternalTable(const Table* table, size_type index) const {
        return ((*(_data.x + index)) == table) || (*(_data.x + index))->Equals(table);
    }

    void Datum::Clear() {
        if (!_isDataInternal) {
            throw std::out_of_range("Cannot modify external data, invalid clear operation."s);
        }

        assert((_type != DatumType::Table) && (_type != DatumType::ExternalTable));

        if (_type == DatumType::String) {
            for (auto i = size_type(0); i < _size; ++i) {
                (_data.s + i)->~basic_string();
            }
        } else if (_type == DatumType::InternalTable) {
            for (auto i = size_type(0); i < _size; ++i) {
                (*(_data.t + i))->_parent = nullptr;
                (_data.t + i)->reset();
            }
        }

        _size = size_type(0);
    }

    void Datum::SetType(DatumType type) {
        if ((type == DatumType::Unknown) || (_type == DatumType::Table) || (_type == DatumType::ExternalTable)) {
            throw std::invalid_argument("Cannot set datum type to "s + ToStringDatumType(type) + "."s);
        }

        if (type == _type) {
            return;
        }

        if (_type != DatumType::Unknown) {
            throw std::invalid_argument("Datum type is already set."s);
        }

        _type = type;
    }

    void Datum::Resize(size_type size) {
        if (!_isDataInternal) {
            throw std::out_of_range("Cannot modify external data, invalid resize operation."s);
        }

        if (_type == DatumType::Unknown) {
            throw std::logic_error("Cannot resize datum with type Unknown."s);
        }

        assert((_type != DatumType::Table) && (_type != DatumType::ExternalTable));

        _capacity = size;

        if (_type == DatumType::String) {
            for (auto i = _capacity; i < _size; ++i) {
                (_data.s + i)->~basic_string();
            }
        } else if (_type == DatumType::InternalTable) {
            for (auto i = size_type(0); i < _size; ++i) {
                (*(_data.t + i))->_parent = nullptr;
                (_data.t + i)->reset();
            }
        }

        if (_capacity == size_type(0)) {
            std::free(_data.vp);
            _data.vp = nullptr;
            _size = _capacity;
            return;
        }

        void* data = std::realloc(_data.vp, TypeSize() * _capacity);
        assert(data != nullptr);
        _data.vp = data;

        switch (_type) {

        case DatumType::Integer:
            for (auto index = _size; index < _capacity; ++index) {
                *(_data.i + index) = Integer{};
            }
            break;

        case DatumType::Float:
            for (auto index = _size; index < _capacity; ++index) {
                *(_data.f + index) = Float{};
            }
            break;

        case DatumType::String:
            for (auto index = _size; index < _capacity; ++index) {
                new (_data.s + index) String{};
            }
            break;

        case DatumType::Vector:
            for (auto index = _size; index < _capacity; ++index) {
                new (_data.v + index) Vector{};
            }
            break;

        case DatumType::Matrix:
            for (auto index = _size; index < _capacity; ++index) {
                new (_data.m + index) Matrix{};
            }
            break;

        case DatumType::Pointer:
            for (auto index = _size; index < _capacity; ++index) {
                *(_data.p + index) = nullptr;
            }
            break;

        case DatumType::InternalTable:
            for (auto index = _size; index < _capacity; ++index) {
                new (_data.p + index) InternalTablePointer{};
            }
            break;

        }

        _size = _capacity;
    }

    void Datum::Reserve(size_type capacity) {
        if (!_isDataInternal || (capacity <= _capacity) || (_type == DatumType::Unknown)) {
            return;
        }

        assert((_type != DatumType::Table) && (_type != DatumType::ExternalTable));

        void* data = std::realloc(_data.vp, TypeSize() * capacity);
        assert(data != nullptr);
        _data.vp = data;
        _capacity = capacity;
    }

    void Datum::ShrinkToFit(size_type minCapacity) {
        if (_size == _capacity) {
            return;
        }

        assert(_isDataInternal);

        _capacity = std::max(std::min(_capacity, minCapacity), _size);

        if (_capacity == size_type(0)) {
            std::free(_data.vp);
            _data.vp = nullptr;
            return;
        }

        void* data = (_type == DatumType::Unknown) ? nullptr : std::realloc(_data.vp, TypeSize() * _capacity);
        assert(data != nullptr);
        _data.vp = data;
    }

    void Datum::SetStorage(DatumType type, void* array, size_type size, bool isConst) {
        if (type == DatumType::InternalTable) {
            throw std::invalid_argument("Cannot set storage of type "s + ToStringDatumType(type) + "!"s);
        }

        if (_isDataInternal) {
            Clear();
            ShrinkToFit();
        }

        _type = (type == DatumType::Table) ? DatumType::ExternalTable : type;
        _data.vp = array;
        _size = size;
        _capacity = size;
        _isDataInternal = false;
        _isDataExternalConst = isConst;
    }

    bool Datum::IsElementRetrievable(size_type index) const {
        if (index >= _size) {
            return false;
        }

        if (ApparentType() == DatumType::Table) {
            if (ActualType() == DatumType::InternalTable) {
                return (_data.t + index)->operator bool();
            } else {
                assert(ActualType() != DatumType::Table);
                return (*(_data.x + index)) != nullptr;
            }
        }

        return true;
    }

    void Datum::ValidateSetElement(DatumType type, size_type index) {
        using namespace std::literals::string_literals;

        if (IsDataExternalConst()) {
            throw std::logic_error("Datum is const, cannot set element!"s);
        }

        if (!IsType(type)) {
            throw std::invalid_argument("Datum is not "s + ToStringDatumType(type) + " type."s);
        }

        if (_size <= index) {
            throw std::out_of_range("Index out of range, cannot set "s + ToStringDatumType(type) + " element."s);
        }
    }

    void Datum::SetElement(const Table& element, size_type index) {
        if (_isDataInternal) {
            SetElementFromMove<InternalTablePointer>(DatumType::InternalTable, std::forward<InternalTablePointer>(element.Clone()), index);
        } else {
            ValidateSetElement(DatumType::ExternalTable, index);
            *(*GetElementPointerNoCheck<ExternalTablePointer>(index)) = element;
        }
    }

    void Datum::SetElement(Table&& element, size_type index) {
        if (_isDataInternal) {
            SetElementFromMove<InternalTablePointer>(
                DatumType::InternalTable,
                std::forward<InternalTablePointer>(std::make_unique<Table>(std::forward<Table>(element))),
                index
            );
        } else {
            ValidateSetElement(DatumType::ExternalTable, index);
            *(*GetElementPointerNoCheck<ExternalTablePointer>(index)) = std::move(element);
        }
    }

    void Datum::PushBack(const Table& element) {
        EmplaceBackTable(std::forward<InternalTablePointer>(element.Clone()));
    }

    void Datum::PushBack(Table&& element) {
        EmplaceBackTable(std::forward<InternalTablePointer>(std::make_unique<Table>(std::forward<Table>(element))));
    }

    void Datum::PopBack() {
        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid pop back operation."s);
        }

        assert((_type != DatumType::Table) && (_type != DatumType::ExternalTable));

        if (_size > size_type(0)) {
            --_size;

            if (_type == DatumType::String) {
                (_data.s + _size)->~basic_string();
            }

            if (_type == DatumType::InternalTable) {
                (*(_data.t + _size))->_parent = nullptr;
                (_data.t + _size)->reset();
            }
        }
    }

    bool Datum::Find(Pointer element, size_type& index) const {
        if (_type == DatumType::Pointer) {
            for (index = size_type(0); index < _size; ++index) {
                if (IsElementEqualToPointer(element, index)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool Datum::Find(const InternalTablePointer& element, size_type& index) const {
        if (ApparentType() == DatumType::Table) {
            for (index = size_type(0); index < _size; ++index) {
                if (IsElementEqualToTable(element.get(), index)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool Datum::Find(const Table& element, size_type& index) const {
        if (ApparentType() == DatumType::Table) {
            for (index = size_type(0); index < _size; ++index) {
                if (IsElementEqualToTable(&element, index)) {
                    return true;
                }
            }
        }

        return false;
    }

    void Datum::PromulgateParent() {
        assert(ActualType() != DatumType::Table);

        if (ActualType() == DatumType::InternalTable) {
            for (auto i = size_type(0); i < _size; ++i) {
                (*(_data.t + i))->_parent = _parent;
            }
        }
    }

    bool Datum::RemoveAt(size_type index) {
        if ((index >= _size) || !_isDataInternal) {
            return false;
        }

        assert(_data.vp != nullptr);
        assert((_type != DatumType::Unknown) && (_type != DatumType::Table) && (_type != DatumType::ExternalTable));

        if (_type == DatumType::String) {
            (_data.s + index)->~basic_string();
        }

        if (_type == DatumType::InternalTable) {
            (*(_data.t + index))->_parent = nullptr;
            (_data.t + index)->reset();
        }

        auto diff = _size - index - 1;
        if (diff > size_type(0)) {
            std::memmove(_data.bp + (index * TypeSize()), _data.bp + ((index + 1) * TypeSize()), diff * TypeSize());
        }

        --_size;
        return true;
    }

    void Datum::PushBackFromString(const String& element) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid push back operation."s);
        }

        if (_type == DatumType::Unknown) {
            throw std::logic_error("Cannot push back element to datum of type Unknown if conversion from string is required."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        auto index = _size++;
        SetElementFromString(element, index);
    }

    void Datum::SetElementFromString(const String& element, size_type index) {
        if (IsDataExternalConst()) {
            throw std::logic_error("Datum is const, cannot set element from string!"s);
        }

        if (index >= _size) {
            throw std::out_of_range("Index out of range, cannot make element from string."s);
        }

        switch (_type) {

        case FieaGameEngine::Datum::DatumType::Integer:
            if (sscanf_s(element.c_str(), "%d", _data.i + index) < 1) {
                throw std::invalid_argument("String conversion to Integer failed."s);
            }
            break;

        case FieaGameEngine::Datum::DatumType::Float:
            if (sscanf_s(element.c_str(), "%F", _data.f + index) < 1) {
                throw std::invalid_argument("String conversion to Float failed."s);
            }
            break;

        case FieaGameEngine::Datum::DatumType::String:
            *(_data.s + index) = element;
            break;

        case FieaGameEngine::Datum::DatumType::Vector:
            {
                Vector v{};
                if (sscanf_s(element.c_str(), VECTOR_STRING_FORMAT.c_str(), &(v.x), &(v.y), &(v.z), &(v.w)) < Vector::length()) {
                    throw std::invalid_argument("String conversion to Vector failed.");
                }
                *(_data.v + index) = v;
            }
            break;

        case FieaGameEngine::Datum::DatumType::Matrix:
            {
                Matrix m{};
                Matrix::row_type& r0 = m[0];
                Matrix::row_type& r1 = m[1];
                Matrix::row_type& r2 = m[2];
                Matrix::row_type& r3 = m[3];
                if (sscanf_s(
                    element.c_str(), MATRIX_STRING_FORMAT.c_str(),
                    &(r0.x), &(r0.y), &(r0.z), &(r0.w),
                    &(r1.x), &(r1.y), &(r1.z), &(r1.w),
                    &(r2.x), &(r2.y), &(r2.z), &(r2.w),
                    &(r3.x), &(r3.y), &(r3.z), &(r3.w))
                < (Matrix::length() * Matrix::row_type::length())) {
                    throw std::invalid_argument("String conversion to Matrix failed.");
                }
                *(_data.m + index) = m;
            }
            break;

        default:
            throw std::invalid_argument("Invalid datum type to make elements from strings."s);

        }
    }

    typename Datum::String Datum::ElementToString(size_type index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range, cannot make string out of element."s);
        }

        String s{};

        switch (_type) {

        case DatumType::Integer:
            s = std::to_string(*(_data.i + index));
            break;

        case DatumType::Float:
            s = std::to_string(*(_data.f + index));
            break;

        case DatumType::String:
            s = *(_data.s + index);
            break;

        case DatumType::Vector:
            {
                char buffer[250];
                const Vector& v = *(_data.v + index);
                sprintf_s(buffer, 250, VECTOR_STRING_FORMAT.c_str(), v.x, v.y, v.z, v.w);
                s = String{buffer};
            }
            break;

        case DatumType::Matrix:
            {
                char buffer[850];
                const Matrix& m = *(_data.m + index);
                const Matrix::row_type& r0 = m[0];
                const Matrix::row_type& r1 = m[1];
                const Matrix::row_type& r2 = m[2];
                const Matrix::row_type& r3 = m[3];
                sprintf_s(
                    buffer, 850, MATRIX_STRING_FORMAT.c_str(),
                    r0.x, r0.y, r0.z, r0.w,
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w,
                    r3.x, r3.y, r3.z, r3.w);
                s = String{buffer};
            }
            break;

        case DatumType::Pointer:
            s = (*(_data.p + index))->ToString();
            break;

        case DatumType::InternalTable:
            s = (*(_data.t + index))->ToString();
            break;

        case DatumType::ExternalTable:
            s = (*(_data.x + index))->ToString();
            break;

        }

        return s;
    }

    typename Datum::String Datum::ToString() const {
        String s = "[ "s;

        if (_size >= size_type(1)) {
            s += ElementToString(size_type(0));
        }

        for (auto index = size_type(1); index < _size; ++index) {
            s += ", "s + ElementToString(index);
        }

        s += " ]"s;

        return s;
    }

    void Datum::swap(Datum& other) {
        using std::swap;

        GrowCapacityFunctorType func = std::move(_growCapacityFunctor);
        _growCapacityFunctor = std::move(other._growCapacityFunctor);
        other._growCapacityFunctor = std::move(func);

        DatumType type = std::move(_type);
        _type = std::move(other._type);
        other._type = std::move(type);

        swap(_size, other._size);
        swap(_capacity, other._capacity);
        swap(_isDataInternal, other._isDataInternal);

        void* vp = _data.vp;
        _data.vp = other._data.vp;
        other._data.vp = vp;
    }

    void Datum::PushBack(InternalTablePointer element) {
        if (!element) {
            throw std::invalid_argument("Cannot push null table into datum!"s);
        }

        element->_parent = _parent;
        EmplaceBackTable(std::forward<InternalTablePointer>(element));
    }

    Datum::DatumType StringToDatumType(const Datum::String& str) {
        using DatumType = Datum::DatumType;

        Datum::String cmp = string_tolower(string_trim(str));
        uint8_t size = static_cast<uint8_t>(DatumType::__SIZE);

        for (auto i = uint8_t(0); i < size; ++i) {
            DatumType type = static_cast<DatumType>(i);
            if (cmp == ToStringDatumType(type)) {
                return type;
            }
        }

        return Datum::DatumType::Unknown;
    }

    bool Datum::IsTruthy(size_type index) const {
        if (index >= _size) {
            return false;
        }

        switch (ActualType()) {

        case DatumType::Integer:
            return (*(_data.i + index)) != 0;

        case DatumType::Float:
            return std::fabsf(*(_data.f + index)) > std::numeric_limits<Float>::epsilon();

        case DatumType::String:
            {
                const String& s = *(_data.s + index);
                return !(s.empty()) && (string_tolower(string_trim(s)) != FALSE_STRING_LOWER);
            }

        case DatumType::Pointer:
            return (*(_data.p + index)) != nullptr;

        case DatumType::InternalTable:
            return (_data.t + index)->operator bool();

        case DatumType::ExternalTable:
            return (*(_data.x + index)) != nullptr;

        }

        return true;
    }
}
