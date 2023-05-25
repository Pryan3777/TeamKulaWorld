#pragma once
#include "IEventArgs.h"

namespace LibraryDesktopTests {
    class HeapedIntEventArgs final : public FieaGameEngine::IEventArgs {
        RTTI_DECLARATIONS(HeapedIntEventArgs, FieaGameEngine::IEventArgs);

    public:
        inline explicit HeapedIntEventArgs(int data = 0) : _data{new int{data}} {};
        inline HeapedIntEventArgs(const HeapedIntEventArgs& other) : _data{new int{other.GetData()}} {}
        inline HeapedIntEventArgs(HeapedIntEventArgs&& other) noexcept : _data{other._data} { other._data = nullptr; }
        inline HeapedIntEventArgs& operator=(const HeapedIntEventArgs& other) { SetData(other.GetData()); return *this; }
        HeapedIntEventArgs& operator=(HeapedIntEventArgs&&) noexcept;
        inline ~HeapedIntEventArgs() { delete _data; }

        inline std::unique_ptr<IEventArgs> Clone() const override { return std::make_unique<HeapedIntEventArgs>(*this); }

        inline int& Data() { return *_data; }
        inline int GetData() const { return *_data; }
        inline void SetData(int data) { (*_data) = data; }

    private:
        int* _data;

    };
}
