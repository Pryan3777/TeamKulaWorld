#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "JsonNestedObjectParseHelper.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine {
    JsonParseCoordinator::JsonParseCoordinator(WrapperSharedPointer wrapper) : ParseCoordinator{std::forward<WrapperSharedPointer>(wrapper)} {
        PushFrontHelper(std::make_unique<JsonNestedObjectParseHelper>());
    }

    JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept {
        if (this != &other) {
            ParseCoordinator::operator=(std::forward<JsonParseCoordinator>(other));
            _helpers = std::move(other._helpers);
            _transmuters = std::move(other._transmuters);
        }

        return *this;
    }

    JsonParseCoordinator::~JsonParseCoordinator() {
        for (auto& helper : _helpers) {
            helper->Cleanup();
        }

        for (auto& transmuter : _transmuters) {
            transmuter->Cleanup();
        }
    }

    void JsonParseCoordinator::DeserializeIntoWrapper(std::istream* payload) {
        if ((payload == nullptr) || !(payload->good())) {
            throw std::invalid_argument("Failed to open stream for deserialization!"s);
        }

        Json::Value root{};

        try {
            (*payload) >> root;
        } catch (const std::exception& e) { throw std::invalid_argument(e.what()); }

        if (!ParseRoot(root)) {
            throw std::runtime_error("Failed to parse object!"s);
        }
    }

    bool JsonParseCoordinator::ParseRoot(const Json::Value& json) {
        if (!json.isObject()) {
            return false;
        }

        return ParseObject(json);
    }

    bool JsonParseCoordinator::ParseObject(const Json::Value& json) {
        const auto& names = json.getMemberNames();

        for (const auto& n : names) {
            IJsonValueTransmuter::pair_type member = std::make_pair(n, std::cref(json[n]));
            for (auto& transmuter : _transmuters) {
                member = (transmuter->Transmute(*_wrapper, member.first, member.second));
            }

            const auto& name = member.first;
            const auto& value = member.second.get();
            bool isArray = value.isArray();
            bool isNotParsed = false;

            for (auto i = Json::ArrayIndex(0); isArray ? (i < value.size()) : (i == 0); ++i) {
                if (!ParseElement(name, isArray ? value[i] : value, isArray, i)) {
                    isNotParsed = true;
                    break;
                }
            }

            if (!(_transmuters.IsEmpty())) {
                auto transmuteItr = _transmuters.end();
                for (--transmuteItr; transmuteItr > _transmuters.begin(); --transmuteItr) {
                    member = (*transmuteItr)->EndTransmutation(*_wrapper, member.first, member.second);
                }
                member = (*transmuteItr)->EndTransmutation(*_wrapper, member.first, member.second);
            }

            if (isNotParsed) {
                return false;
            }
        }

        return true;
    }

    bool JsonParseCoordinator::ParseElement(const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) {
        for (const auto& helper : _helpers) {
            if (helper->TryParse(*_wrapper, key, value, isArrayElement, index)) {
                return helper->EndParse(*_wrapper, key);
            }
        }

        return false;
    }

    std::shared_ptr<ParseCoordinator> JsonParseCoordinator::Clone() const {
        auto clone = std::make_shared<JsonParseCoordinator>((_wrapper.operator bool()) ? _wrapper->Create() : WrapperSharedPointer{});
        clone->SetWrapper(clone->_wrapper);

        for (const auto& helper : _helpers) {
            clone->PushBackHelper(helper->Create());
        }

        for (const auto& transmuter : _transmuters) {
            clone->PushBackTransmuter(transmuter->Create());
        }

        return clone;
    }

    bool JsonParseCoordinator::IsContainingHelper(const IJsonParseHelper* helper) const {
        return IsContainingHelper(helper->TypeIdInstance());
    }

    const IJsonParseHelper* JsonParseCoordinator::GetHelper(RTTI::IdType helperTypeId) const {
        for (auto position = _helpers.begin(); position != _helpers.end(); ++position) {
            if ((*position)->TypeIdInstance() == helperTypeId) {
                return position->get();
            }
        }

        return nullptr;
    }

    bool JsonParseCoordinator::PushBackHelper(std::unique_ptr<IJsonParseHelper> helper) { return PushHelper(std::move(helper), true); }
    bool JsonParseCoordinator::PushFrontHelper(std::unique_ptr<IJsonParseHelper> helper) { return PushHelper(std::move(helper), false); }

    bool JsonParseCoordinator::PushHelper(std::unique_ptr<IJsonParseHelper> helper, bool isPushBack) {
        if (IsContainingHelper(helper.get())) {
            return false;
        }

        helper->Initialize();
        auto position = isPushBack ? _helpers.EmplaceBack(std::move(helper)) : _helpers.EmplaceFront(std::move(helper));
        return position != _helpers.end();
    }

    bool JsonParseCoordinator::RemoveHelper(RTTI::IdType helperTypeId) {
        for (auto position = _helpers.begin(); position != _helpers.end(); ++position) {
            if ((*position)->TypeIdInstance() == helperTypeId) {
                (*position)->Cleanup();
                return _helpers.Remove(position);
            }
        }

        return false;
    }

    const IJsonValueTransmuter* JsonParseCoordinator::GetTransmuter(RTTI::IdType transmuterTypeId) const {
        for (auto position = _transmuters.begin(); position != _transmuters.end(); ++position) {
            if ((*position)->TypeIdInstance() == transmuterTypeId) {
                return position->get();
            }
        }

        return nullptr;
    }

    bool JsonParseCoordinator::PushBackTransmuter(std::unique_ptr<IJsonValueTransmuter> transmuter) {
        if (IsContainingTransmuter(transmuter.get())) {
            return false;
        }

        transmuter->Initialize();
        auto position = _transmuters.EmplaceBack(std::move(transmuter));
        return position != _transmuters.end();
    }

    bool JsonParseCoordinator::RemoveTransmuter(RTTI::IdType transmuterTypeId) {
        for (auto position = _transmuters.begin(); position != _transmuters.end(); ++position) {
            if ((*position)->TypeIdInstance() == transmuterTypeId) {
                (*position)->Cleanup();
                return _transmuters.Remove(position);
            }
        }

        return false;
    }
}
