#pragma once
#include <functional>
#include <memory>
#include <json/json.h>
#include "ParseCoordinator.h"
#include "RTTI.h"
#include "Stack.h"

namespace FieaGameEngine {
    class IJsonValueTransmuter : public RTTI {
        RTTI_DECLARATIONS(IJsonValueTransmuter, RTTI);

    protected:
        using Wrapper = ParseCoordinator::Wrapper;

    public:
        using pair_type = std::pair<Json::String, std::reference_wrapper<const Json::Value>>;

        virtual ~IJsonValueTransmuter() = default;

        virtual inline void Initialize() {}
        virtual inline void Cleanup() {}

        [[nodiscard]] virtual pair_type Transmute(Wrapper& wrapper, const Json::String& key, const Json::Value& value) = 0;
        [[nodiscard]] virtual pair_type EndTransmutation(Wrapper& wrapper, const Json::String& key, const Json::Value& value) = 0;

        [[nodiscard]] virtual std::unique_ptr<IJsonValueTransmuter> Create() const = 0;

    protected:
        class TransmuteHistory {

        public:
            inline TransmuteHistory(const pair_type& input, bool isAbleToTransmute) : _input{input}, _isAbleToTransmute{isAbleToTransmute} {}

            inline const pair_type& Input() const { return _input; }
            inline bool IsAbleToTransmute() const { return _isAbleToTransmute; }

        private:
            pair_type _input;
            bool _isAbleToTransmute;

        };

        Stack<std::unique_ptr<TransmuteHistory>> _history{};

    };
}
