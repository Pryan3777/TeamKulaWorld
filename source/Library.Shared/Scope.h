#pragma once
#include "RTTI.h"
#include "Datum.h"
#include "HashMap.h"
#include "Vector.h"
#include "Factory.h"

namespace FieaGameEngine {
    class Scope : public RTTI {
        RTTI_DECLARATIONS(Scope, RTTI);

    public:
        // Datum is our friend :)
        friend class Datum;

        using ScopeUniquePointer = Datum::InternalTablePointer;
        using String = Datum::String;
        using key_type = String;
        using GrowCapacityFunctorType = Vector<Datum>::GrowCapacityFunctorType;
        using size_type = Vector<Datum>::size_type;

    protected:
        inline static constexpr std::size_t MAX_HASH_VAL = 31;

        HashMap<key_type, Datum> _map{MAX_HASH_VAL};
        using value_type = decltype(_map)::value_type;
        Vector<value_type*> _array{};
        Scope* _parent{nullptr};

    public:
        /// <summary>
        /// Random-access iterator of the scope. This iterator is non-const.
        /// </summary>
        using iterator = decltype(_map)::iterator;

        /// <summary>
        /// Random-access iterator of the scope. This iterator is const.
        /// </summary>
        using const_iterator = decltype(_map)::const_iterator;

    private:
        /// <summary>
        /// Helper function which completes the move operation of scopes.
        /// </summary>
        void FinalizeMove(Scope&& other) noexcept;

        /// <summary>
        /// Appends the given child to the given datum. The child will now be owned by this scope.
        /// The datum must be validated before this function is called.
        /// </summary>
        Scope& AppendScope(Datum& datum, ScopeUniquePointer&& child);

        // TODO comment
        void ParentDatumsToThis();

        // TODO comment
        void FullClear();

    protected:
        /// <summary>
        /// Functor to perform operations on every nested scope. Returns true if the iteration should terminate early.
        /// </summary>
        using IsNestedScopeForEachBreakingFunctor = std::function<bool(Datum&, Scope&, size_type)>;

        /// <summary>
        /// Performs the given operation on each nested scope. If the functor returns `true`, the iteration terminates early.
        /// </summary>
        void ForEachNestedScope(IsNestedScopeForEachBreakingFunctor isForEachBreakingFunctor);

        /// <summary>
        /// Returns a validated datum to which a scope can be appended. Throws an exception if such a datum cannot be retrieved.
        /// </summary>
        Datum& ValidatedAppendScopeDatum(const key_type& key);

        /// <summary>
        /// Finds the datum and the datum's index which contains the given scope. If no such datum is found,
        /// the pair will be `{ nullptr, Size() }`;
        /// </summary>
        [[nodiscard]] std::pair<Datum*, size_type> FindContainingDatum(const Scope& scope);

    public:
        static const key_type THIS_KEY;

        /// <summary>
        /// Default constructor.
        /// </summary>
        /// <param name="capacity"> - Sets the initial capacity of the internal vector used by the scope.</param>
        /// <param name="growCapacityFunctor"> - Sets the growth strategy used by the internal vector of the scope.</param>
        explicit Scope(size_type capacity = size_type(0), GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        Scope(const Scope&);
        Scope(Scope&&) noexcept;
        Scope& operator=(const Scope&);
        Scope& operator=(Scope&&) noexcept;
        virtual ~Scope();

        /// <summary>
        /// Creates a datum within the scope at the given key. If the key is already in use,
        /// the existing datum is returned.
        /// </summary>
        /// <param name="...args"> - Optional construction parameters for the datum.</param>
        /// <returns>A reference to the newly appended (or previously existing) datum.</returns>
        template <typename... Args> Datum& Append(const key_type& key, Args&&... args);

        /// <summary>
        /// Syntactical sugar for Append. This is unlike the const version of this operator,
        /// and is also unlike the size_type version of this operator.
        /// </summary>
        Datum& operator[](const key_type& key);

        /// <summary>
        /// Returns a constant reference to the datum at the given key. Will NOT create a new datum
        /// if the given key is not already in use.
        /// </summary>
        const Datum& operator[](const key_type& key) const;

        /// <summary>
        /// Returns a reference to the datum at the given ordinal index. Will NOT create a new datum
        /// if the given index is not already in use.
        /// </summary>
        Datum& operator[](size_type index);

        /// <summary>
        /// Returns a constant reference to the datum at the given ordinal index. Will NOT create a new datum
        /// if the given index is not already in use.
        /// </summary>
        const Datum& operator[](size_type index) const;

        [[nodiscard]] bool operator==(const Scope& other) const;
        [[nodiscard]] bool operator!=(const Scope& other) const;

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] Datum& At(const key_type& key);

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] const Datum& CAt(const key_type& key) const;

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] const Datum& At(const key_type& key) const;

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] Datum& At(size_type index);

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] const Datum& CAt(size_type index) const;

        /// <returns>Reference to the datum at the given position, with bounds checking.</returns>
        [[nodiscard]] const Datum& At(size_type index) const;

        /// <summary>
        /// Appends a new scope at the given position. If the given key is already in use, a new scope is appended
        /// alongside any existing ones at the same key.
        /// </summary>
        /// <returns>Reference to the newly appended scope.</returns>
        Scope& AppendScope(const key_type& key, size_type capacity = size_type(0), GrowCapacityFunctorType growCapacityFunctor = DefaultGrowCapacity{});

        /// <summary>
        /// Appends a new scope at the given position. If the given key is already in use, a new scope is appended
        /// alongside any existing ones at the same key.
        /// </summary>
        /// <param name="classname"> - Name of the Scope-derived class which should be instantiated.</param>
        /// <returns>Reference to the newly appended scope.</returns>
        Scope& AppendScope(const key_type& key, const std::string& classname);

        /// <returns>Does the scope contain a datum mapped to the given key?</returns>
        [[nodiscard]] bool IsContainingKey(const key_type& key) const;

        /// <summary>
        /// Searches within the local scope to find an element with the given key.
        /// </summary>
        [[nodiscard]] iterator Find(const key_type& key);

        /// <summary>
        /// Searches within the local scope to find an element with the given key.
        /// </summary>
        [[nodiscard]] const_iterator CFind(const key_type& key) const;

        /// <summary>
        /// Searches within the local scope to find an element with the given key.
        /// </summary>
        [[nodiscard]] const_iterator Find(const key_type& key) const;

        /// <summary>
        /// Searches within the local scope and all parent scopes to find an element mapped to the given key.
        /// </summary>
        /// <param name="outputContainingScope"> - Output parameter. Pointer to the scope which locally contains the element.</param>
        [[nodiscard]] virtual Datum* Search(const key_type& key, Scope*& outputContainingScope);

        /// <summary>
        /// Searches within the local scope and all parent scopes to find an element mapped to the given key.
        /// </summary>
        [[nodiscard]] Datum* Search(const key_type& key);

        /// <summary>
        /// Searches within the local scope and all parent scopes to find an element mapped to the given key.
        /// </summary>
        [[nodiscard]] const Datum* CSearch(const key_type& key) const;

        /// <summary>
        /// Searches within the local scope and all parent scopes to find an element mapped to the given key.
        /// </summary>
        [[nodiscard]] const Datum* Search(const key_type& key) const;

        /// <summary>
        /// Attaches the given scope to this scope as a child. Cannot be used to create cyclic dependencies,
        /// and if the child had a different parent it will be removed from that parent before being added
        /// to this one.
        /// </summary>
        /// <param name="outputDidDetachFromOtherParent">
        /// - Output parameter. Set to `true` if the given scope had a parent previously, set to `false` otherwise.
        /// </param>
        /// <returns>
        /// Nothing, but will throw an exception if attaching the scope as a child would result in a cyclic dependency,
        /// such as if the given scope is this scope or an ancestor of this scope.
        /// </returns>
        void AttachAsChild(const key_type& key, Scope&& child, bool& outputDidDetachFromOtherParent);

        /// <summary>
        /// Attaches the given scope to this scope as a child. Cannot be used to create cyclic dependencies,
        /// and if the child had a different parent it will be removed from that parent before being added
        /// to this one.
        /// </summary>
        /// <returns>
        /// Nothing, but will throw an exception if attaching the scope as a child would result in a cyclic dependency,
        /// such as if the given scope is this scope or an ancestor of this scope.
        /// </returns>
        void AttachAsChild(const key_type& key, Scope&& child);

        /// <summary>
        /// Attaches the given scope to this scope as a child. Cannot be used to create cyclic dependencies,
        /// and if the child had a different parent it will be removed from that parent before being added
        /// to this one.
        /// </summary>
        /// <param name="outputDidDetachFromOtherParent">
        /// - Output parameter. Set to `true` if the given scope had a parent previously, set to `false` otherwise.
        /// </param>
        /// <returns>
        /// Nothing, but will throw an exception if attaching the scope as a child would result in a cyclic dependency,
        /// such as if the given scope is this scope or an ancestor of this scope.
        /// </returns>
        void AttachAsChild(const key_type& key, ScopeUniquePointer&& child, bool& outputDidDetachFromOtherParent);

        /// <summary>
        /// Attaches the given scope to this scope as a child. Cannot be used to create cyclic dependencies,
        /// and if the child had a different parent it will be removed from that parent before being added
        /// to this one.
        /// </summary>
        /// <returns>
        /// Nothing, but will throw an exception if attaching the scope as a child would result in a cyclic dependency,
        /// such as if the given scope is this scope or an ancestor of this scope.
        /// </returns>
        void AttachAsChild(const key_type& key, ScopeUniquePointer&& child);

        /// <summary>
        /// If this scope is nested, removes the scope from its parent.
        /// </summary>
        void DetachFromTree();

        /// <summary>
        /// Attempts to set the parent to the given pointer. This only works if the parent is currently null.
        /// </summary>
        /// <returns>true if the parent was set, false otherwise.</returns>
        bool TrySetParent(Scope* parent);

        /// <returns>Is this scope a descendant of the given scope?</returns>
        [[nodiscard]] bool IsDescendantOf(const Scope& other) const;

        /// <returns>Is this scope the same as or a descendant of the given scope?</returns>
        [[nodiscard]] bool IsThisOrDescendantOf(const Scope& scope) const;

        /// <returns>Is this scope an ancestor of the given scope?</returns>
        [[nodiscard]] bool IsAncestorOf(const Scope& other) const;

        /// <returns>Is this scope the same as or an ancestor of the given scope?</returns>
        [[nodiscard]] bool IsThisOrAncestorOf(const Scope& scope) const;

        /// <summary>
        /// Clones this object.
        /// </summary>
        /// <returns>A pointer to the heap allocated clone. The caller is responsible for deleting this object.</returns>
        [[nodiscard]] virtual ScopeUniquePointer Clone() const;

        /// <summary>
        /// Clears the scope.
        /// </summary>
        virtual void Clear(); // TODO TrueClear()

        /// <returns>Is this RTTI object equivalent to the given object?</returns>
        [[nodiscard]] virtual bool Equals(const RTTI* other) const override;

        /// <returns>The datum at the given index converted to a string.</returns>
        [[nodiscard]] std::string ElementToString(size_type index) const;

        /// <returns>The scope converted to a string.</returns>
        [[nodiscard]] virtual std::string ToString() const override;

        /// <returns>Pointer to this scope's parent.</returns>
        [[nodiscard]] Scope* Parent() const;

        /// <returns>Number of unique datums within this scope.</returns>
        [[nodiscard]] size_type Size() const;

        /// <returns>Is the scope empty?</returns>
        [[nodiscard]] bool IsEmpty() const;

        void swap(Scope& other);

        /// <returns>Iterator to the beginning of this scope.</returns>
        [[nodiscard]] iterator begin();

        /// <returns>Iterator to the beginning of this scope. Dereference operations using this iterator will return const references.</returns>
        [[nodiscard]] const_iterator cbegin() const;

        /// <returns>Iterator to the beginning of this scope. Dereference operations using this iterator will return const references.</returns>
        [[nodiscard]] const_iterator begin() const;

        /// <returns>Iterator to the end of this scope. This iterator is outside the bounds of the scope.</returns>
        [[nodiscard]] iterator end();

        /// <returns>Iterator to the end of this scope. This iterator is outside the bounds of the scope.</returns>
        [[nodiscard]] const_iterator cend() const;

        /// <returns>Iterator to the end of this scope. This iterator is outside the bounds of the scope.</returns>
        [[nodiscard]] const_iterator end() const;
    };

    /// <summary>
    /// Swaps the contents of the scopes.
    /// </summary>
    void swap(Scope& lhs, Scope& rhs);

    FACTORY(Scope, Scope);

}

#include "Scope.inl"
