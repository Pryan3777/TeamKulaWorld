#pragma once
#include "Datum.h"

namespace FieaGameEngine {
    template <typename T> inline void Datum::InitializeFromList(const std::initializer_list<T>& list) {
        Reserve(list.size());

        for (const auto& element : list) {
            PushBack(element);
        }
    }

    template <typename T> inline Datum& Datum::AssignFromScalarCopy(DatumType type, T scalar) {
        BeginAssignFromScalar(type);
        _data.vp = new T{scalar};
        return *this;
    }

    template <typename T> inline Datum& Datum::AssignFromScalarReference(DatumType type, const T& scalar) {
        BeginAssignFromScalar(type);
        _data.vp = new T{scalar};
        return *this;
    }

    template <typename T> inline Datum& Datum::AssignFromScalarForward(DatumType type, T&& scalar) {
        BeginAssignFromScalar(type);
        _data.vp = new T{std::forward<T>(scalar)};
        return *this;
    }

    template <typename T> inline Datum& Datum::AssignFromList(DatumType type, const std::initializer_list<T>& list) {
        if (_isDataInternal) {
            Clear();
            ShrinkToFit();
        }

        _type = type;
        _isDataInternal = true;
        _isDataExternalConst = false;
        Reserve(list.size());

        for (const auto& element : list) {
            PushBack(element);
        }

        return *this;
    }

    template <typename T> inline bool Datum::IsEqualToScalar(DatumType type, const T& scalar) const {
        if ((_type != type) || (_size != size_type(1))) {
            return false;
        }

        return *(reinterpret_cast<T*>(_data.vp)) == scalar;
    }

    template <typename T> inline T* Datum::GetElementPointerNoCheck(size_type index) {
        return reinterpret_cast<T*>(_data.bp + (TypeSize() * index));
    }

    template <typename T> inline const T* Datum::CGetElementPointerNoCheck(size_type index) const {
        return const_cast<Datum*>(this)->GetElementPointerNoCheck<T>(index);
    }

    template <typename T> inline T* Datum::GetElementPointer(DatumType type, size_type index, bool isPointerMutable) {
        using namespace std::literals::string_literals;

        if (isPointerMutable && IsDataExternalConst()) {
            throw std::logic_error("Datum is const, cannot retrieve mutable element!"s);
        }

        if (_type != type) {
            throw std::logic_error("Datum is not "s + ToStringDatumType(type) + " type."s);
        }

        if ((_data.vp == nullptr) || (_size <= index)) {
            throw std::out_of_range("Index out of range, cannot get "s + ToStringDatumType(type) + " element."s);
        }

        return GetElementPointerNoCheck<T>(index);
    }

    template <typename T> inline const T* Datum::CGetElementPointer(DatumType type, size_type index) const {
        return const_cast<Datum*>(this)->GetElementPointer<T>(type, index, false);
    }

    template <typename T> inline void Datum::SetElementFromCopy(DatumType type, T element, size_type index) {
        ValidateSetElement(type, index);
        *GetElementPointerNoCheck<T>(index) = element;
    }

    template <typename T> inline void Datum::SetElementFromReference(DatumType type, const T& element, size_type index) {
        ValidateSetElement(type, index);
        *GetElementPointerNoCheck<T>(index) = element;
    }

    template <typename T> inline void Datum::SetElementFromMove(DatumType type, T&& element, size_type index) {
        ValidateSetElement(type, index);
        *GetElementPointerNoCheck<T>(index) = std::move(element);
    }

    template <typename T> inline void Datum::PushBackCopy(DatumType type, T element) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid push back operation."s);
        }

        if (_type == DatumType::Unknown) {
            assert(_size == size_type(0));
            _type = type;
        }

        if (_type != type) {
            throw std::invalid_argument("Cannot push back "s + ToStringDatumType(type) + " element to non-"s + ToStringDatumType(type) + " datum."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        *GetElementPointerNoCheck<T>(index) = element;
    }

    template <typename... Args> inline void Datum::EmplaceBackString(Args&&... args) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid emplace back operation."s);
        }

        if (_type == DatumType::Unknown) {
            assert(_size == size_type(0));
            _type = DatumType::String;
        }

        if (_type != DatumType::String) {
            throw std::invalid_argument("Cannot push back String element to non-String datum."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        new (_data.s + index) String{std::forward<Args>(args)...};
    }

    template <typename... Args> inline void Datum::EmplaceBackVector(Args&&... args) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid emplace back operation."s);
        }

        if (_type == DatumType::Unknown) {
            assert(_size == size_type(0));
            _type = DatumType::Vector;
        }

        if (_type != DatumType::Vector) {
            throw std::invalid_argument("Cannot push back Vector element to non-Vector datum."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        new (_data.v + index) Vector{std::forward<Args>(args)...};
    }

    template <typename... Args> inline void Datum::EmplaceBackMatrix(Args&&... args) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid emplace back operation."s);
        }

        if (_type == DatumType::Unknown) {
            assert(_size == size_type(0));
            _type = DatumType::Matrix;
        }

        if (_type != DatumType::Matrix) {
            throw std::invalid_argument("Cannot push back Matrix element to non-Matrix datum."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        new (_data.m + index) Matrix{std::forward<Args>(args)...};
    }

    template <typename... Args> inline void Datum::EmplaceBackTable(Args&&... args) {
        using namespace std::literals::string_literals;

        if (!_isDataInternal) {
            throw std::logic_error("Cannot modify external data, invalid emplace back operation."s);
        }

        if (_type == DatumType::Unknown) {
            assert(_size == size_type(0));
            _type = DatumType::InternalTable;
        }

        if (ActualType() != DatumType::InternalTable) {
            throw std::invalid_argument("Cannot push back InternalTable element to non-InternalTable datum."s);
        }

        if (_size == _capacity) {
            Reserve(std::max(_growCapacityFunctor(_size, _capacity), _capacity + 1));
        }

        size_type index = _size++;
        new (_data.t + index) InternalTablePointer{std::forward<Args>(args)...};
    }

    template <typename T> inline bool Datum::Find(DatumType type, const T& element, size_type& index) const {
        if (_type == type) {
            for (index = size_type(0); index < _size; ++index) {
                if (*CGetElementPointerNoCheck<T>(index) == element) {
                    return true;
                }
            }
        }

        return false;
    }

    template <typename T> inline T& Datum::GetElementReference(DatumType type, size_type index) { return *GetElementPointer<T>(type, index, true); }
    template <typename T> inline T Datum::CGetElementCopy(DatumType type, size_type index) const { return *CGetElementPointer<T>(type, index); }
    template <typename T> inline const T& Datum::CGetElementReference(DatumType type, size_type index) const { return *CGetElementPointer<T>(type, index); }

    inline Datum::Datum(DatumType type, size_type capacity, GrowCapacityFunctorType growCapacityFunctor) : Datum{type, capacity, nullptr, growCapacityFunctor} {}
    inline Datum::Datum(DatumType type, size_type capacity, ExternalTablePointer parent, GrowCapacityFunctorType growCapacityFunctor)
        : _type{type}
        , _parent{parent}
        , _growCapacityFunctor{growCapacityFunctor}
    {
        Reserve(capacity);
    }
    inline Datum::Datum(DatumType type, GrowCapacityFunctorType growCapacityFunctor)
        : _type{type}
        , _size{size_type(1)}
        , _capacity{size_type(1)}
        , _growCapacityFunctor{growCapacityFunctor} {}
    inline Datum::Datum(Integer scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Integer, growCapacityFunctor} { _data.i = new Integer{scalar}; }
    inline Datum::Datum(Float scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Float, growCapacityFunctor} { _data.f = new Float{scalar}; }
    inline Datum::Datum(const String& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::String, growCapacityFunctor} { _data.s = new String{scalar}; }
    inline Datum::Datum(String&& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::String, growCapacityFunctor} { _data.s = new String{std::forward<String>(scalar)}; }
    inline Datum::Datum(const Vector& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Vector, growCapacityFunctor} { _data.v = new Vector{scalar}; }
    inline Datum::Datum(Vector&& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Vector, growCapacityFunctor} { _data.v = new Vector{std::forward<Vector>(scalar)}; }
    inline Datum::Datum(const Matrix& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Matrix, growCapacityFunctor} { _data.m = new Matrix{scalar}; }
    inline Datum::Datum(Matrix&& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{DatumType::Matrix, growCapacityFunctor} { _data.m = new Matrix{std::forward<Matrix>(scalar)}; }
    inline Datum::Datum(InternalTablePointer scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{std::forward<InternalTablePointer>(scalar), nullptr, growCapacityFunctor} {}
    inline Datum::Datum(const Table& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{std::make_unique<Table>(scalar), nullptr, growCapacityFunctor} {}
    inline Datum::Datum(Table&& scalar, GrowCapacityFunctorType growCapacityFunctor) : Datum{std::make_unique<Table>(std::forward<Table>(scalar)), nullptr, growCapacityFunctor} {}

    inline Datum::Datum(std::initializer_list<Integer> list) : _type{DatumType::Integer} { InitializeFromList(list); }
    inline Datum::Datum(std::initializer_list<Float> list) : _type{DatumType::Float} { InitializeFromList(list); }
    inline Datum::Datum(std::initializer_list<String> list) : _type{DatumType::String} { InitializeFromList(list); }
    inline Datum::Datum(std::initializer_list<Vector> list) : _type{DatumType::Vector} { InitializeFromList(list); }
    inline Datum::Datum(std::initializer_list<Matrix> list) : _type{DatumType::Matrix} { InitializeFromList(list); }
    inline Datum::Datum(std::initializer_list<Pointer> list) : _type{DatumType::Pointer} { InitializeFromList(list); }

    inline Datum& Datum::operator=(Integer scalar) { return AssignFromScalarCopy(DatumType::Integer, scalar); }
    inline Datum& Datum::operator=(Float scalar) { return AssignFromScalarCopy(DatumType::Float, scalar); }
    inline Datum& Datum::operator=(const String& scalar) { return AssignFromScalarReference(DatumType::String, scalar); }
    inline Datum& Datum::operator=(String&& scalar) { return AssignFromScalarForward(DatumType::String, std::forward<String>(scalar)); }
    inline Datum& Datum::operator=(const Vector& scalar) { return AssignFromScalarReference(DatumType::Vector, scalar); }
    inline Datum& Datum::operator=(Vector&& scalar) { return AssignFromScalarForward(DatumType::Vector, std::forward<Vector>(scalar)); }
    inline Datum& Datum::operator=(const Matrix& scalar) { return AssignFromScalarReference(DatumType::Matrix, scalar); }
    inline Datum& Datum::operator=(Matrix&& scalar) { return AssignFromScalarForward(DatumType::Matrix, std::forward<Matrix>(scalar)); }

    inline Datum& Datum::operator=(std::initializer_list<Integer> list) { return AssignFromList(DatumType::Integer, list); }
    inline Datum& Datum::operator=(std::initializer_list<Float> list) { return AssignFromList(DatumType::Float, list); }
    inline Datum& Datum::operator=(std::initializer_list<String> list) { return AssignFromList(DatumType::String, list); }
    inline Datum& Datum::operator=(std::initializer_list<Vector> list) { return AssignFromList(DatumType::Vector, list); }
    inline Datum& Datum::operator=(std::initializer_list<Matrix> list) { return AssignFromList(DatumType::Matrix, list); }
    inline Datum& Datum::operator=(std::initializer_list<Pointer> list) { return AssignFromList(DatumType::Pointer, list); }

    inline bool Datum::operator==(Integer other) const { return IsEqualToScalar(DatumType::Integer, other); }
    inline bool Datum::operator==(Float other) const { return IsEqualToScalar(DatumType::Float, other); }
    inline bool Datum::operator==(const String& other) const { return IsEqualToScalar(DatumType::String, other); }
    inline bool Datum::operator==(const Vector& other) const { return IsEqualToScalar(DatumType::Vector, other); }
    inline bool Datum::operator==(const Matrix& other) const { return IsEqualToScalar(DatumType::Matrix, other); }
    inline bool Datum::operator!=(const Datum& other) const { return !operator==(other); }
    inline bool Datum::operator!=(Integer other) const { return !operator==(other); }
    inline bool Datum::operator!=(Float other) const { return !operator==(other); }
    inline bool Datum::operator!=(const String& other) const { return !operator==(other); }
    inline bool Datum::operator!=(const Vector& other) const { return !operator==(other); }
    inline bool Datum::operator!=(const Matrix& other) const { return !operator==(other); }
    inline bool Datum::operator!=(const Pointer other) const { return !operator==(other); }
    inline bool Datum::operator!=(const InternalTablePointer& other) const { return !operator==(other); }
    inline bool Datum::operator!=(const Table& other) const { return !operator==(other); }

    inline void Datum::SetAndPromulgateParent(ExternalTablePointer parent) { _parent = parent; PromulgateParent(); }
    inline const typename Datum::ExternalTablePointer Datum::Parent() const { return _parent; }
    inline bool Datum::IsDataInternal() const { return _isDataInternal; }
    inline bool Datum::IsDataExternalConst() const { assert(!(_isDataInternal && _isDataExternalConst)); return _isDataExternalConst; }
    inline bool Datum::IsEmpty() const { return _size == size_type(0); }
    inline const typename Datum::size_type& Datum::Size() const { return _size; }
    inline const typename Datum::size_type& Datum::Capacity() const { return _capacity; }
    inline typename Datum::DatumType Datum::ActualType() const { return _type; }
    inline typename Datum::DatumType Datum::ApparentType() const { return (_type == DatumType::InternalTable || _type == DatumType::ExternalTable) ? DatumType::Table : _type; }
    inline bool Datum::IsType(DatumType type) const { return (type == DatumType::Table) ? (_type == DatumType::InternalTable || _type == DatumType::ExternalTable) : (_type == type); }

    inline void Datum::SetStorage(Integer* array, size_type size, bool isConst) { SetStorage(DatumType::Integer, array, size, isConst); }
    inline void Datum::SetStorage(Float* array, size_type size, bool isConst) { SetStorage(DatumType::Float, array, size, isConst); }
    inline void Datum::SetStorage(String* array, size_type size, bool isConst) { SetStorage(DatumType::String, array, size, isConst); }
    inline void Datum::SetStorage(Vector* array, size_type size, bool isConst) { SetStorage(DatumType::Vector, array, size, isConst); }
    inline void Datum::SetStorage(Matrix* array, size_type size, bool isConst) { SetStorage(DatumType::Matrix, array, size, isConst); }
    inline void Datum::SetPointerStorage(Pointer* array, size_type size, bool isConst) { SetStorage(DatumType::Pointer, array, size, isConst); }
    inline void Datum::SetTableStorage(ExternalTablePointer* array, size_type size, bool isConst) { SetStorage(DatumType::ExternalTable, array, size, isConst); }

    inline typename Datum::Integer& Datum::GetIntegerElement(size_type index) { return GetElementReference<Integer>(DatumType::Integer, index); }
    inline typename Datum::Float& Datum::GetFloatElement(size_type index) { return GetElementReference<Float>(DatumType::Float, index); }
    inline typename Datum::String& Datum::GetStringElement(size_type index) { return GetElementReference<String>(DatumType::String, index); }
    inline typename Datum::Vector& Datum::GetVectorElement(size_type index) { return GetElementReference<Vector>(DatumType::Vector, index); }
    inline typename Datum::Matrix& Datum::GetMatrixElement(size_type index) { return GetElementReference<Matrix>(DatumType::Matrix, index); }
    inline typename Datum::Pointer& Datum::GetPointerElement(size_type index) { return GetElementReference<Pointer>(DatumType::Pointer, index); }
    inline typename Datum::Table& Datum::GetTableElement(size_type index) {
        assert(_type != DatumType::Table);
        return (_type == DatumType::InternalTable)
            ? *(GetElementReference<InternalTablePointer>(DatumType::InternalTable, index))
            : *(GetElementReference<ExternalTablePointer>(DatumType::ExternalTable, index));
    }

    inline typename Datum::Integer Datum::GetIntegerElement(size_type index) const { return CGetIntegerElement(index); }
    inline typename Datum::Float Datum::GetFloatElement(size_type index) const { return CGetFloatElement(index); }
    inline const typename Datum::String& Datum::GetStringElement(size_type index) const { return CGetStringElement(index); }
    inline const typename Datum::Vector& Datum::GetVectorElement(size_type index) const { return CGetVectorElement(index); }
    inline const typename Datum::Matrix& Datum::GetMatrixElement(size_type index) const { return CGetMatrixElement(index); }
    inline const typename Datum::Pointer Datum::GetPointerElement(size_type index) const { return CGetPointerElement(index); }
    inline typename const Datum::Table& Datum::GetTableElement(size_type index) const { return CGetTableElement(index); }

    inline typename Datum::Integer Datum::CGetIntegerElement(size_type index) const { return CGetElementCopy<Integer>(DatumType::Integer, index); }
    inline typename Datum::Float Datum::CGetFloatElement(size_type index) const { return CGetElementCopy<Float>(DatumType::Float, index); }
    inline const typename Datum::String& Datum::CGetStringElement(size_type index) const { return CGetElementReference<String>(DatumType::String, index); }
    inline const typename Datum::Vector& Datum::CGetVectorElement(size_type index) const { return CGetElementReference<Vector>(DatumType::Vector, index); }
    inline const typename Datum::Matrix& Datum::CGetMatrixElement(size_type index) const { return CGetElementReference<Matrix>(DatumType::Matrix, index); }
    inline const typename Datum::Pointer Datum::CGetPointerElement(size_type index) const { return CGetElementCopy<Pointer>(DatumType::Pointer, index); }
    inline const typename Datum::Table& Datum::CGetTableElement(size_type index) const {
        assert(ActualType() != DatumType::Table);
        return (ActualType() == DatumType::InternalTable)
            ? *(CGetElementReference<InternalTablePointer>(DatumType::InternalTable, index))
            : *(CGetElementReference<ExternalTablePointer>(DatumType::ExternalTable, index));
    }

    inline typename Datum::Table& Datum::operator[](size_type index) { return GetTableElement(index); }
    inline const typename Datum::Table& Datum::operator[](size_type index) const { return CGetTableElement(index); }

    inline void Datum::SetElement(Integer element, size_type index) { SetElementFromCopy(DatumType::Integer, element, index); }
    inline void Datum::SetElement(Float element, size_type index) { SetElementFromCopy(DatumType::Float, element, index); }
    inline void Datum::SetElement(const String& element, size_type index) { SetElementFromReference(DatumType::String, element, index); }
    inline void Datum::SetElement(String&& element, size_type index) { SetElementFromMove(DatumType::String, std::forward<String>(element), index); }
    inline void Datum::SetElement(const Vector& element, size_type index) { SetElementFromReference(DatumType::Vector, element, index); }
    inline void Datum::SetElement(Vector&& element, size_type index) { SetElementFromMove(DatumType::Vector, std::forward<Vector>(element), index); }
    inline void Datum::SetElement(const Matrix& element, size_type index) { SetElementFromReference(DatumType::Matrix, element, index); }
    inline void Datum::SetElement(Matrix&& element, size_type index) { SetElementFromMove(DatumType::Matrix, std::forward<Matrix>(element), index); }
    inline void Datum::SetElement(Pointer element, size_type index) { SetElementFromCopy(DatumType::Pointer, element, index); }

    inline void Datum::PushBack(Integer element) { PushBackCopy(DatumType::Integer, element); }
    inline void Datum::PushBack(Float element) { PushBackCopy(DatumType::Float, element); }
    inline void Datum::PushBack(const String& element) { EmplaceBackString(element); }
    inline void Datum::PushBack(String&& element) { EmplaceBackString(std::forward<String>(element)); }
    inline void Datum::PushBack(const Vector& element) { EmplaceBackVector(element); }
    inline void Datum::PushBack(Vector&& element) { EmplaceBackVector(std::forward<Vector>(element)); }
    inline void Datum::PushBack(const Matrix& element) { EmplaceBackMatrix(element); }
    inline void Datum::PushBack(Matrix&& element) { EmplaceBackMatrix(std::forward<Matrix>(element)); }
    inline void Datum::PushBack(Pointer element) { PushBackCopy(DatumType::Pointer, element); }

    inline typename Datum::Integer& Datum::FrontInteger() { return GetIntegerElement(size_type(0)); }
    inline typename Datum::Float& Datum::FrontFloat() { return GetFloatElement(size_type(0)); }
    inline typename Datum::String& Datum::FrontString() { return GetStringElement(size_type(0)); }
    inline typename Datum::Vector& Datum::FrontVector() { return GetVectorElement(size_type(0)); }
    inline typename Datum::Matrix& Datum::FrontMatrix() { return GetMatrixElement(size_type(0)); }
    inline typename Datum::Pointer& Datum::FrontPointer() { return GetPointerElement(size_type(0)); }
    inline typename Datum::Table& Datum::FrontTable() { return GetTableElement(size_type(0)); }
    inline typename Datum::Integer Datum::FrontInteger() const { return CFrontInteger(); }
    inline typename Datum::Float Datum::FrontFloat() const { return CFrontFloat(); }
    inline const typename Datum::String& Datum::FrontString() const { return CFrontString(); }
    inline const typename Datum::Vector& Datum::FrontVector() const { return CFrontVector(); }
    inline const typename Datum::Matrix& Datum::FrontMatrix() const { return CFrontMatrix(); }
    inline typename Datum::Pointer Datum::FrontPointer() const { return CFrontPointer(); }
    inline const typename Datum::Table& Datum::FrontTable() const { return CFrontTable(); }
    inline typename Datum::Integer Datum::CFrontInteger() const { return CGetIntegerElement(size_type(0)); }
    inline typename Datum::Float Datum::CFrontFloat() const { return CGetFloatElement(size_type(0)); }
    inline const typename Datum::String& Datum::CFrontString() const { return CGetStringElement(size_type(0)); }
    inline const typename Datum::Vector& Datum::CFrontVector() const { return CGetVectorElement(size_type(0)); }
    inline const typename Datum::Matrix& Datum::CFrontMatrix() const { return CGetMatrixElement(size_type(0)); }
    inline typename Datum::Pointer Datum::CFrontPointer() const { return CGetPointerElement(size_type(0)); }
    inline const typename Datum::Table& Datum::CFrontTable() const { return CGetTableElement(size_type(0)); }
    inline typename Datum::Integer& Datum::BackInteger() { return GetIntegerElement(_size - size_type(1)); }
    inline typename Datum::Float& Datum::BackFloat() { return GetFloatElement(_size - size_type(1)); }
    inline typename Datum::String& Datum::BackString() { return GetStringElement(_size - size_type(1)); }
    inline typename Datum::Vector& Datum::BackVector() { return GetVectorElement(_size - size_type(1)); }
    inline typename Datum::Matrix& Datum::BackMatrix() { return GetMatrixElement(_size - size_type(1)); }
    inline typename Datum::Pointer& Datum::BackPointer() { return GetPointerElement(_size - size_type(1)); }
    inline typename Datum::Table& Datum::BackTable() { return GetTableElement(_size - size_type(1)); }
    inline typename Datum::Integer Datum::BackInteger() const { return CBackInteger(); }
    inline typename Datum::Float Datum::BackFloat() const { return CBackFloat(); }
    inline const typename Datum::String& Datum::BackString() const { return CBackString(); }
    inline const typename Datum::Vector& Datum::BackVector() const { return CBackVector(); }
    inline const typename Datum::Matrix& Datum::BackMatrix() const { return CBackMatrix(); }
    inline typename Datum::Pointer Datum::BackPointer() const { return CBackPointer(); }
    inline const typename Datum::Table& Datum::BackTable() const { return CBackTable(); }
    inline typename Datum::Integer Datum::CBackInteger() const { return CGetIntegerElement(_size - size_type(1)); }
    inline typename Datum::Float Datum::CBackFloat() const { return CGetFloatElement(_size - size_type(1)); }
    inline const typename Datum::String& Datum::CBackString() const { return CGetStringElement(_size - size_type(1)); }
    inline const typename Datum::Vector& Datum::CBackVector() const { return CGetVectorElement(_size - size_type(1)); }
    inline const typename Datum::Matrix& Datum::CBackMatrix() const { return CGetMatrixElement(_size - size_type(1)); }
    inline typename Datum::Pointer Datum::CBackPointer() const { return CGetPointerElement(_size - size_type(1)); }
    inline const typename Datum::Table& Datum::CBackTable() const { return CGetTableElement(_size - size_type(1)); }

    inline bool Datum::Find(Integer element, size_type& index) const { return Find(DatumType::Integer, element, index); }
    inline bool Datum::Find(Float element, size_type& index) const { return Find(DatumType::Float, element, index); }
    inline bool Datum::Find(const String& element, size_type& index) const { return Find(DatumType::String, element, index); }
    inline bool Datum::Find(const Vector& element, size_type& index) const { return Find(DatumType::Vector, element, index); }
    inline bool Datum::Find(const Matrix& element, size_type& index) const { return Find(DatumType::Matrix, element, index); }

    inline bool Datum::IsContaining(Integer element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(Float element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(const String& element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(const Vector& element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(const Matrix& element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(Pointer element) const { size_type _; return Find(element, _); }

    inline bool Datum::Remove(Integer element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(Float element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(const String& element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(const Vector& element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(const Matrix& element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(Pointer element) { size_type index; Find(element, index); return RemoveAt(index); }

    inline typename Datum::String Datum::ToString(size_type index) const { return ElementToString(index); }

    inline const Datum::GrowCapacityFunctorType& Datum::GetGrowCapacityFunctor() const { return _growCapacityFunctor; }
    inline void Datum::SetGrowCapacityFunctor(GrowCapacityFunctorType growCapacityFunctor) { _growCapacityFunctor = growCapacityFunctor; }

    inline void swap(Datum& lhs, Datum& rhs) { lhs.swap(rhs); }

    inline void Datum::SetElement(InternalTablePointer element, size_type index) {
        assert(_isDataInternal);
        assert((ActualType() != DatumType::Table) && (ActualType() != DatumType::ExternalTable));
        SetElementFromMove(DatumType::InternalTable, std::forward<InternalTablePointer>(element), index);
    }

    inline bool Datum::IsContaining(const InternalTablePointer& element) const { size_type _; return Find(element, _); }
    inline bool Datum::IsContaining(const Table& element) const { size_type _; return Find(element, _); }

    inline bool Datum::Remove(const InternalTablePointer& element) { size_type index; Find(element, index); return RemoveAt(index); }
    inline bool Datum::Remove(const Table& element) { size_type index; Find(element, index); return RemoveAt(index); }
}
