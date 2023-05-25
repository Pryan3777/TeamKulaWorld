#pragma once
#include "ParseCoordinator.h"
#include <fstream>
#include <sstream>

namespace FieaGameEngine {
    inline ParseCoordinator::Wrapper::Wrapper() : Wrapper{nullptr} {}
    inline ParseCoordinator::Wrapper::Wrapper(ParseCoordinator* coordinator) : _coordinator{coordinator}, _depth{depth_type(0)} {}

    inline void ParseCoordinator::Wrapper::HandleSubobjectNameAndIndex(const std::string&, bool, std::size_t) {}
    inline bool ParseCoordinator::Wrapper::IncrementDepth() { return IncrementDepth(std::string{}); }
    inline bool ParseCoordinator::Wrapper::IncrementDepth(const std::string& subobjectName, bool isArray, std::size_t arrayIndex) {
        ++_depth;
        HandleSubobjectNameAndIndex(subobjectName, isArray, arrayIndex);
        return true;
    }

    inline typename ParseCoordinator::Wrapper::depth_type ParseCoordinator::Wrapper::Depth() const { return _depth; }
    inline ParseCoordinator* ParseCoordinator::Wrapper::Coordinator() const { return _coordinator; }

    inline ParseCoordinator::ParseCoordinator(ParseCoordinator&& other) noexcept
        : _wrapper{std::move(other._wrapper)}
    {
        _wrapper->_coordinator = this;
    }

    inline void ParseCoordinator::DeserializeIntoWrapperFromString(const std::string& payload) { auto stream = std::istringstream{payload}; return DeserializeIntoWrapper(&stream); }
    inline void ParseCoordinator::DeserializeIntoWrapperFromFile(const std::string& filename) { auto stream = std::ifstream{filename}; return DeserializeIntoWrapper(&stream); }

    inline typename ParseCoordinator::WrapperSharedPointer ParseCoordinator::GetWrapper() { return _wrapper; }
    inline const typename ParseCoordinator::WrapperSharedPointer& ParseCoordinator::GetWrapper() const { return _wrapper; }
}
