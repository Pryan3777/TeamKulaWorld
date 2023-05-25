#pragma once
#include <cstdint>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include "DefaultGrowCapacity.h"
#include "RTTI.h"

namespace FieaGameEngine {
    // Forward declaration.
    class Scope;

    /// <summary>
    /// Wraps an array of data. For supported types please see DatumTypes.
    /// </summary>
    class Datum final {

    public:
        // Scope is our friend. :)
        friend class Scope;

        // so is Attributed. :)
        friend class Attributed;

        /// <summary>
        /// Integer type.
        /// </summary>
        using Integer = int;

        /// <summary>
        /// Float type.
        /// </summary>
        using Float = float;

        /// <summary>
        /// String type.
        /// </summary>
        using String = std::string;

        /// <summary>
        /// Vector type.
        /// </summary>
        using Vector = glm::vec4;

        /// <summary>
        /// Matrix type.
        /// </summary>
        using Matrix = glm::mat4;

        /// <summary>
        /// Pointer type.
        /// </summary>
        using Pointer = RTTI*;

        /// <summary>
        /// Table type.
        /// </summary>
        using Table = Scope;

        /// <summary>
        /// Table pointer type for internal storage.
        /// </summary>
        using InternalTablePointer = std::unique_ptr<Table>;

        /// <summary>
        /// Table pointer type for external storage.
        /// </summary>
        using ExternalTablePointer = Table*;

        /// <summary>
        /// Size type, used for indicies and for storing size and capacity.
        /// </summary>
        using size_type = std::size_t;

        /// <summary>
        /// Describes the types supported for storage by Datum.
        /// </summary>
        enum class DatumType : std::uint8_t {
            Unknown,
            Integer,
            Float,
            String,
            Vector,
            Matrix,
            Pointer,
            Table,
            InternalTable,
            ExternalTable,
            __SIZE
        };

        /// <summary>
        /// Takes as parameters the datum's current size and then the datum's current capacity (in that order), and then returns what the new capacity should be.
        /// </summary>
        using GrowCapacityFunctorType = std::function<size_type(size_type, size_type)>;

    private:
        /// <summary>
        /// Union to all pointer types, thereby simplifying storage algorithms.
        /// </summary>
        union DatumValues final {
            Integer* i;
            Float* f;
            String* s;
            Vector* v;
            Matrix* m;
            Pointer* p;
            InternalTablePointer* t;
            ExternalTablePointer* x;
            std::byte* bp;
            void* vp;
        };

        /// <summary>
        /// Stores sizes of all supported types. Used for pointer arithmetic.
        /// </summary>
        static const size_type TYPE_SIZES[];

        /// <summary>
        /// Stores string representations of supported type names.
        /// </summary>
        static const String TO_STRING_DATUM_TYPE[];

        /// <summary>
        /// Data currently allocated.
        /// </summary>
        DatumValues _data{nullptr};

        /// <summary>
        /// Parent of this datum.
        /// </summary>
        ExternalTablePointer _parent{nullptr};

        /// <summary>
        /// Current number of elements stored in the datum.
        /// </summary>
        size_type _size{size_type(0)};

        /// <summary>
        /// Current maximum number of elements capable of being stored by the datum.
        /// </summary>
        size_type _capacity{size_type(0)};

        /// <summary>
        /// Current type of the datum.
        /// </summary>
        DatumType _type{DatumType::Unknown};

        /// <summary>
        /// Is the data internally stored? If false, then data is external, and this datum has no ownership of the data.
        /// </summary>
        bool _isDataInternal{true};

        /// <summary>
        /// Is the externally stored data const? This should never be true if the data is internal.
        /// </summary>
        bool _isDataExternalConst{false};

        /// <summary>
        /// Strategy this datum employs to gain new capacity when full. Uses the `DefaultGrowCapacity` functor type by default.
        /// </summary>
        GrowCapacityFunctorType _growCapacityFunctor{DefaultGrowCapacity{}};

        /// <summary>
        /// Retrieves the size of the datum's current type.
        /// </summary>
        [[nodiscard]] inline constexpr size_type TypeSize() const { return TYPE_SIZES[static_cast<int>(_type)]; }

        /// <summary>
        /// Constructor for setting the datum's type, initial size, initial capacity, and capacity growth strategy,
        /// without assigning any elements.
        /// </summary>
        explicit Datum(DatumType type, GrowCapacityFunctorType growCapacityFunctor);

        /// <summary>
        /// Constructor for setting the datum's type, initial size, initial capacity, and capacity growth strategy,
        /// without assigning any elements.
        /// </summary>
        explicit Datum(DatumType type, ExternalTablePointer parent, GrowCapacityFunctorType growCapacityFunctor);

        /// <summary>
        /// Helper function which initializes the datum from an initializer list.
        /// </summary>
        template <typename T> void InitializeFromList(const std::initializer_list<T>& list);

        /// <summary>
        /// Helper function which begins the process for assigning from a scalar;
        /// </summary>
        void BeginAssignFromScalar(DatumType type);

        /// <summary>
        /// Helper function which assigns the datum from a scalar.
        /// </summary>
        template <typename T> [[nodiscard]] Datum& AssignFromScalarCopy(DatumType type, T scalar);

        /// <summary>
        /// Helper function which assigns the datum from a scalar.
        /// </summary>
        template <typename T> [[nodiscard]] Datum& AssignFromScalarReference(DatumType type, const T& scalar);

        /// <summary>
        /// Helper function which assigns the datum from a scalar.
        /// </summary>
        template <typename T> [[nodiscard]] Datum& AssignFromScalarForward(DatumType type, T&& scalar);

        /// <summary>
        /// Helper function which assigns the datum from an initializer list.
        /// </summary>
        template <typename T> [[nodiscard]] Datum& AssignFromList(DatumType type, const std::initializer_list<T>& list);

        /// <summary>
        /// Helper function which checks equivalance to a scalar.
        /// </summary>
        template <typename T> [[nodiscard]] bool IsEqualToScalar(DatumType type, const T& scalar) const;

        /// <summary>
        /// Helper function which checks equivalance to a Pointer element.
        /// </summary>
        [[nodiscard]] bool IsElementEqualToPointer(Pointer pointer, size_type index) const;

        /// <summary>
        /// Helper function which checks equivalance to a Table element.
        /// </summary>
        [[nodiscard]] bool IsElementEqualToTable(const Table* table, size_type index) const;

        /// <summary>
        /// Helper function which checks equivalance to a Table element.
        /// </summary>
        [[nodiscard]] bool IsElementEqualToInternalTable(const Table* table, size_type index) const;

        /// <summary>
        /// Helper function which checks equivalance to a Table element.
        /// </summary>
        [[nodiscard]] bool IsElementEqualToExternalTable(const Table* table, size_type index) const;

        /// <summary>
        /// Helper function for setting external storage. After this method completed data will always be marked external.
        /// </summary>
        void SetStorage(DatumType type, void* array, size_type size, bool isConst);

        /// <summary>
        /// Helper function which returns a mutable pointer to the element in question without doing any checks.
        /// </summary>
        template <typename T> [[nodiscard]] T* GetElementPointerNoCheck(size_type index);

        /// <summary>
        /// Helper function which returns an immutable pointer to the element in question without doing any checks.
        /// </summary>
        template <typename T> [[nodiscard]] const T* CGetElementPointerNoCheck(size_type index) const;

        /// <summary>
        /// Helper function which gets an element as a pointer.
        /// </summary>
        template <typename T> [[nodiscard]] T* GetElementPointer(DatumType type, size_type index, bool isPointerMutable);

        /// <summary>
        /// Helper function which gets an element as an immutable pointer.
        /// </summary>
        template <typename T> [[nodiscard]] const T* CGetElementPointer(DatumType type, size_type index) const;

        /// <summary>
        /// Helper function which gets an element as a mutable reference.
        /// </summary>
        template <typename T> [[nodiscard]] T& GetElementReference(DatumType type, size_type index);

        /// <summary>
        /// Helper function which gets an element as a copy.
        /// </summary>
        template <typename T> [[nodiscard]] T CGetElementCopy(DatumType type, size_type index) const;

        /// <summary>
        /// Helper function which gets an element as an immutable reference.
        /// </summary>
        template <typename T> [[nodiscard]] const T& CGetElementReference(DatumType type, size_type index) const;

        /// <summary>
        /// Helper function which validates the input for a set method.
        /// </summary>
        void ValidateSetElement(DatumType type, size_type index);

        /// <summary>
        /// Helper function which sets an element at the given index using a copy.
        /// </summary>
        template <typename T> void SetElementFromCopy(DatumType type, T element, size_type index);

        /// <summary>
        /// Helper function which sets an element at the given index using a reference.
        /// </summary>
        template <typename T> void SetElementFromReference(DatumType type, const T& element, size_type index);

        /// <summary>
        /// Helper function which sets an element at the given index by moving an rvalue reference.
        /// </summary>
        template <typename T> void SetElementFromMove(DatumType type, T&& element, size_type index);

        /// <summary>
        /// Helper function for pushing back a copy of the given element.
        /// </summary>
        template <typename T> void PushBackCopy(DatumType type, T element);

        /// <summary>
        /// Helper function for finding a generic element.
        /// </summary>
        template <typename T> bool Find(DatumType type, const T& element, size_type& index) const;

        // TODO - comment
        void SetAndPromulgateParent(ExternalTablePointer parent);

        // TODO - comment
        void PromulgateParent();

    public:
        /// <summary>
        /// String format used to create and parse string representations of Vector types.
        /// </summary>
        static const String VECTOR_STRING_FORMAT;

        /// <summary>
        /// String format used to create and parse string representations of Vector types.
        /// </summary>
        static const String VECTOR_STRING_FORMAT_PREFIX;

        /// <summary>
        /// String format used to create and parse string representations of Matrix types.
        /// </summary>
        static const String MATRIX_STRING_FORMAT;

        /// <summary>
        /// String format used to create and parse string representations of Matrix types.
        /// </summary>
        static const String MATRIX_STRING_FORMAT_PREFIX;

        /// <summary>
        /// Default constructor. Sets type to Unknown.
        /// </summary>
        Datum() = default;

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Datum(const Datum&);

        /// <summary>
        /// Move constructor.
        /// </summary>
        Datum(Datum&&) noexcept;

        /// <summary>
        /// Copy assignment.
        /// </summary>
        Datum& operator=(const Datum&);

        /// <summary>
        /// Move assignment.
        /// </summary>
        Datum& operator=(Datum&&) noexcept;

        /// <summary>
        /// Destructor.
        /// </summary>
        ~Datum();

        /// <summary>
        /// Constructor for setting the datum's type, and optionally its initial capacity and capacity growth strategy,
        /// without assigning any elements.
        /// </summary>
        explicit Datum(DatumType type, size_type capacity = size_type(0), ExternalTablePointer parent = nullptr, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Constructor for setting the datum's type, and optionally its initial capacity and capacity growth strategy,
        /// without assigning any elements.
        /// </summary>
        Datum(DatumType type, size_type capacity, GrowCapacityFunctorType growCapacityFunctor);

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Integer value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Integer scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Float value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Float scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar String value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(const String& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for moving a scalar String value into the datum. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(String&& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Vector value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(const Vector& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for moving a scalar Vector value into the datum. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Vector&& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Matrix value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(const Matrix& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for moving a scalar Matrix value into the datum. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Matrix&& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Pointer value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Pointer scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Table value. Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(InternalTablePointer scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Table value. Creates a unique pointer from a copy of the given table.
        /// Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(const Table& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Implicit constructor for assigning the datum to a scalar Table value. Creates a unique pointer by moving from the given rvalue reference.
        /// Optionally will accept a capacity growth strategy.
        /// </summary>
        Datum(Table&& scalar, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Constructor for assigning the datum to a scalar Table value. Optionally will accept a capacity growth strategy.
        /// The external table pointer will set the parent of both this datum and the Table.
        /// </summary>
        Datum(InternalTablePointer scalar, ExternalTablePointer parent, GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<Integer> list);

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<Float> list);

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<String> list);

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<Vector> list);

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<Matrix> list);

        /// <summary>
        /// Explicit constructor for initializing the datum using an initializer list.
        /// </summary>
        explicit Datum(std::initializer_list<Pointer> list);

        /// <summary>
        /// Assigns the datum to a scalar Integer value.
        /// </summary>
        Datum& operator=(Integer scalar);

        /// <summary>
        /// Assigns the datum to a scalar Float value.
        /// </summary>
        Datum& operator=(Float scalar);

        /// <summary>
        /// Assigns the datum to a scalar String value.
        /// </summary>
        Datum& operator=(const String& scalar);

        /// <summary>
        /// Moves a scalar String value into the datum.
        /// </summary>
        Datum& operator=(String&& scalar);

        /// <summary>
        /// Assigns the datum to a scalar Vector value.
        /// </summary>
        Datum& operator=(const Vector& scalar);

        /// <summary>
        /// Moves a scalar Vector value into the datum.
        /// </summary>
        Datum& operator=(Vector&& scalar);

        /// <summary>
        /// Assigns the datum to a scalar Matrix value.
        /// </summary>
        Datum& operator=(const Matrix& scalar);

        /// <summary>
        /// Moves a scalar Matrix value into the datum.
        /// </summary>
        Datum& operator=(Matrix&& scalar);

        /// <summary>
        /// Assigns the datum to a scalar Pointer value.
        /// </summary>
        Datum& operator=(Pointer scalar);

        /// <summary>
        /// Assigns the datum to a scalar Table value.
        /// </summary>
        Datum& operator=(InternalTablePointer scalar);

        /// <summary>
        /// Assigns the datum to a scalar Table value. Creates a unique pointer from a copy of the given table.
        /// </summary>
        Datum& operator=(const Table& scalar);

        /// <summary>
        /// Assigns the datum to a scalar Table value. Creates a unique pointer by moving from the given rvalue reference.
        /// </summary>
        Datum& operator=(Table&& scalar);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<Integer> list);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<Float> list);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<String> list);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<Vector> list);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<Matrix> list);

        /// <summary>
        /// Assigns the datum using an initializer list.
        /// </summary>
        Datum& operator=(std::initializer_list<Pointer> list);

        /// <summary>
        /// Compares the datum to another datum.
        /// </summary>
        [[nodiscard]] bool operator==(const Datum& other) const;

        /// <summary>
        /// Compares the datum to an Integer.
        /// </summary>
        [[nodiscard]] bool operator==(Integer other) const;

        /// <summary>
        /// Compares the datum to a Float.
        /// </summary>
        [[nodiscard]] bool operator==(Float other) const;

        /// <summary>
        /// Compares the datum to a String.
        /// </summary>
        [[nodiscard]] bool operator==(const String& other) const;

        /// <summary>
        /// Compares the datum to a Vector.
        /// </summary>
        [[nodiscard]] bool operator==(const Vector& other) const;

        /// <summary>
        /// Compares the datum to a Matrix.
        /// </summary>
        [[nodiscard]] bool operator==(const Matrix& other) const;

        /// <summary>
        /// Compares the datum to a Pointer.
        /// </summary>
        [[nodiscard]] bool operator==(const Pointer other) const;

        /// <summary>
        /// Compares the datum to a Table.
        /// </summary>
        [[nodiscard]] bool operator==(const InternalTablePointer& other) const;

        /// <summary>
        /// Compares the datum to a Table.
        /// </summary>
        [[nodiscard]] bool operator==(const Table& other) const;

        /// <summary>
        /// Inverse comparison of a datum to another datum.
        /// </summary>
        [[nodiscard]] bool operator!=(const Datum& other) const;

        /// <summary>
        /// Inverse comparison of a datum to an Integer.
        /// </summary>
        [[nodiscard]] bool operator!=(Integer other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Float.
        /// </summary>
        [[nodiscard]] bool operator!=(Float other) const;

        /// <summary>
        /// Inverse comparison of a datum to a String.
        /// </summary>
        [[nodiscard]] bool operator!=(const String& other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Vector.
        /// </summary>
        [[nodiscard]] bool operator!=(const Vector& other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Matrix.
        /// </summary>
        [[nodiscard]] bool operator!=(const Matrix& other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Pointer.
        /// </summary>
        [[nodiscard]] bool operator!=(const Pointer other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Table.
        /// </summary>
        [[nodiscard]] bool operator!=(const InternalTablePointer& other) const;

        /// <summary>
        /// Inverse comparison of a datum to a Table.
        /// </summary>
        [[nodiscard]] bool operator!=(const Table& other) const;

        /// <returns>Parent of this datum.</returns>
        [[nodiscard]] const ExternalTablePointer Parent() const;

        /// <returns>Does the datum have ownership of the memory it represents?</returns>
        [[nodiscard]] bool IsDataInternal() const;

        /// <returns>Does the datum NOT have ownership of the memory it represents AND is that memory const?</returns>
        [[nodiscard]] bool IsDataExternalConst() const;

        /// <returns>Does the datum have any elements in it?</returns>
        [[nodiscard]] bool IsEmpty() const;

        /// <returns>Current number of elements in the datum.</returns>
        [[nodiscard]] const size_type& Size() const;

        /// <returns>Current maximum number of elements the datum can contain.</returns>
        [[nodiscard]] const size_type& Capacity() const;

        /// <returns>The actual type of this datum. Currently only differs from `ApparentType()` if the actual type is either `InternalTable` or `ExternalTable`.</returns>
        [[nodiscard]] DatumType ActualType() const;

        /// <returns>The "apparent" type of this datum. Currently only used to return `Table` if the actual type is either `InternalTable` or `ExternalTable`.</returns>
        [[nodiscard]] DatumType ApparentType() const;

        /// <returns>Is the datum's type equivalent to the given type? Returns true if `Table` is given even if the actual types are `InternalTable` or `ExternalTable`.</returns>
        [[nodiscard]] bool IsType(DatumType type) const;

        /// <summary>
        /// Removes all elements from the datum. Does not affect capacity. This is only possible if `IsDataInternal() == true`.
        /// </summary>
        void Clear();

        /// <summary>
        /// Sets the type of the datum. The datum cannot be set to Unknown. If the datum's type is not Unknown before this method is invoked, an exception will be thrown.
        /// </summary>
        void SetType(DatumType);

        /// <summary>
        /// Resizes the datum. This is only possible if the datum's type is not Unknown and if `IsDataInternal() == true`.
        /// </summary>
        void Resize(size_type size);

        /// <summary>
        /// Reserves capacity. This is only possible if the datum's type is not Unknown and if `IsDataInternal() == true`.
        /// </summary>
        void Reserve(size_type capacity);

        /// <summary>
        /// Sets capacity to `Size()`, or to a larger optional value if `minCapacity > Size()`. This is only possible if `IsDataInternal() == true`.
        /// </summary>
        void ShrinkToFit(size_type minCapacity = size_type(0));

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetStorage(Integer* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetStorage(Float* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetStorage(String* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetStorage(Vector* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetStorage(Matrix* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetPointerStorage(Pointer* array, size_type size, bool isConst = false);

        /// <summary>
        /// Sets the datum to external storage. After this method completes `IsDataInternal()` will be `false`.
        /// </summary>
        void SetTableStorage(ExternalTablePointer* array, size_type size, bool isConst = false);

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer& GetIntegerElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float& GetFloatElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] String& GetStringElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Vector& GetVectorElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Matrix& GetMatrixElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer& GetPointerElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Table& GetTableElement(size_type index = size_type(0));

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer GetIntegerElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float GetFloatElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& GetStringElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& GetVectorElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& GetMatrixElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Pointer GetPointerElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& GetTableElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer CGetIntegerElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float CGetFloatElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& CGetStringElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& CGetVectorElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& CGetMatrixElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Pointer CGetPointerElement(size_type index = size_type(0)) const;

        /// <summary>
        /// Gets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& CGetTableElement(size_type index = size_type(0)) const;

        // TODO - Comment
        bool IsElementRetrievable(size_type index = size_type(0)) const; // TODO - TryGetElement, TryFront, TryBack

        /// <summary>
        /// Syntactic sugar for accessing nested table elements.
        /// </summary>
        Table& operator[](size_type index);

        /// <summary>
        /// Syntactic sugar for accessing nested table elements.
        /// </summary>
        const Table& operator[](size_type index) const;

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(Integer element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(Float element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(const String& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(String&& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(const Vector& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(Vector&& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(const Matrix& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(Matrix&& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. The type of the element must match the type of the datum.
        /// </summary>
        void SetElement(Pointer element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. This method only works for internal storage.
        /// </summary>
        void SetElement(InternalTablePointer element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. This method only works for external storage.
        /// </summary>
        void SetElement(const Table& element, size_type index = size_type(0));

        /// <summary>
        /// Sets the element at the given index. This method only works for external storage.
        /// </summary>
        void SetElement(Table&& element, size_type index = size_type(0));

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Integer element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Float element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(const String& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(String&& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(const Vector& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Vector&& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(const Matrix& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Matrix&& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Pointer element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(InternalTablePointer element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(const Table& element);

        /// <summary>
        /// Adds the element to the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        void PushBack(Table&& element);

        /// <summary>
        /// Constructs the element in-place at the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        template <typename... Args> void EmplaceBackString(Args&&... args);

        /// <summary>
        /// Constructs the element in-place at the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        template <typename... Args> void EmplaceBackVector(Args&&... args);

        /// <summary>
        /// Constructs the element in-place at the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        template <typename... Args> void EmplaceBackMatrix(Args&&... args);

        /// <summary>
        /// Constructs the element in-place at the end of the datum. The type of the element must match the type of the datum,
        /// unless the datum's type is currently Unknown, in which case this will set the type.
        /// </summary>
        template <typename... Args> void EmplaceBackTable(Args&&... args);

        /// <summary>
        /// Removes the last element of the datum.
        /// </summary>
        void PopBack();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer& FrontInteger();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float& FrontFloat();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] String& FrontString();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Vector& FrontVector();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Matrix& FrontMatrix();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer& FrontPointer();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Table& FrontTable();

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer FrontInteger() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float FrontFloat() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& FrontString() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& FrontVector() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& FrontMatrix() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer FrontPointer() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& FrontTable() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer CFrontInteger() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float CFrontFloat() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& CFrontString() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& CFrontVector() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& CFrontMatrix() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer CFrontPointer() const;

        /// <summary>
        /// Gets the first element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& CFrontTable() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer& BackInteger();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float& BackFloat();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] String& BackString();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Vector& BackVector();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Matrix& BackMatrix();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer& BackPointer();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Table& BackTable();

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer BackInteger() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float BackFloat() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& BackString() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& BackVector() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& BackMatrix() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer BackPointer() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& BackTable() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Integer CBackInteger() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Float CBackFloat() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const String& CBackString() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Vector& CBackVector() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Matrix& CBackMatrix() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] Pointer CBackPointer() const;

        /// <summary>
        /// Gets the last element of the datum. The type of the element must match the type of the datum.
        /// </summary>
        [[nodiscard]] const Table& CBackTable() const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(Integer element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(Float element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(const String& element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(const Vector& element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(const Matrix& element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(Pointer element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(const InternalTablePointer& element, size_type& index) const;

        /// <summary>
        /// Returns true if the given element is in the datum, false otherwise. If the element is found, `index` is populated with that element's index.
        /// </summary>
        bool Find(const Table& element, size_type& index) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(Integer element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(Float element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(const String& element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(const Vector& element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(const Matrix& element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(Pointer element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(const InternalTablePointer& element) const;

        /// <summary>
        /// Does the datum contain the given element?
        /// </summary>
        [[nodiscard]] bool IsContaining(const Table& element) const;

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(Integer element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(Float element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(const String& element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(const Vector& element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(const Matrix& element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(Pointer element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(const InternalTablePointer& element);

        /// <summary>
        /// Removes the first occurrence of the given element.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool Remove(const Table& element);

        /// <summary>
        /// Removes the element at the given index.
        /// </summary>
        /// <returns>True if an element was removed, false otherwise.</returns>
        bool RemoveAt(size_type index);

        // TODO - Comment
        void PushBackFromString(const String& element);

        /// <summary>
        /// Sets the element at the given index by parsing its value from the given string.
        /// </summary>
        void SetElementFromString(const String& element, size_type index = size_type(0));

        /// <summary>
        /// Converts the element with the given index to a string.
        /// </summary>
        [[nodiscard]] String ElementToString(size_type index = size_type(0)) const;

        /// <summary>
        /// Converts the entire datum into a string.
        /// </summary>
        [[nodiscard]] String ToString() const;

        /// <summary>
        /// Converts the element with the given index to a string.
        /// </summary>
        [[nodiscard]] String ToString(size_type index) const;

        /// <summary>
        /// Swaps the contents of this datum with other.
        /// </summary>
        void swap(Datum& other);

        /// <returns>Functor that is currently used whenever this Datum needs to allocate new space when pushing while full.</returns>
        [[nodiscard]] const GrowCapacityFunctorType& GetGrowCapacityFunctor() const;

        /// <summary>
        /// The given functor will be used to allocate space whenever this Datum needs to allocate new space when pushing while full.
        /// If the functor returns a value less than the current capacity, capacity will still increment by 1.
        /// </summary>
        void SetGrowCapacityFunctor(GrowCapacityFunctorType growCapacityFunctor);

        /// <summary>
        /// Converts a DatumType to a string.
        /// </summary>
        friend inline String ToStringDatumType(DatumType type) { return TO_STRING_DATUM_TYPE[static_cast<int>(type)]; }

        /// <summary>
        /// A value is considered "truthy" if it would be "true" if converted to a boolean. Values are considered "truthy" in the following conditions:
        /// (1) -> If the value is a number, it is "truthy" if it is not 0. Note that if the value is a float, truthiness can be unreliable.
        /// (2) -> If the value is a string, it is "truthy" unless it is empty or is "false" (case insensitive).
        /// (3) -> If the value is a pointer, it is "truthy" unless it is nullptr.
        /// (4) -> If the value is a vector or a matrix, it is always truthy.
        /// </summary>
        /// <returns>Whether the value at the given index is "truthy". If the index is out of bounds, false is returned.</returns>
        bool IsTruthy(size_type index = size_type(0)) const;

    };

    /// <summary>
    /// Swaps the contents of the datums.
    /// </summary>
    void swap(Datum& lhs, Datum& rhs);

    Datum::DatumType StringToDatumType(const Datum::String& str);
}

#include "Datum.inl"
