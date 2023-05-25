#pragma once

namespace FieaGameEngine {
    /// <summary>
    /// Default strategy for collections to allocate new memory when the collection is full.
    /// </summary>
    struct DefaultGrowCapacity final {
        /// <summary>
        /// If `size` is less than `capacity`, returns `capacity`.
        /// If `size` is less than a sentinel value (currently 1024), doubles the size. Otherwise, adds the sentinel value to `size`.
        /// If `size` is 0, returns a default value (currently 8).
        /// </summary>
        /// <returns>The new capacity.</returns>
        [[nodiscard]] std::size_t operator()(std::size_t size, std::size_t capacity) const;
    };
}
