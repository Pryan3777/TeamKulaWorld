#pragma once
#include <json/json.h>
#include "IJsonValueTransmuter.h"
#include "ParseCoordinator.h"
#include "SList.h"
#include "Vector.h"

namespace FieaGameEngine {
    class IJsonParseHelper;
    class JsonNestedObjectParseHelper;
    class JsonKeyTokenParseHelper;

    class JsonParseCoordinator final : public ParseCoordinator {

    public:
        explicit JsonParseCoordinator(WrapperSharedPointer);
        JsonParseCoordinator(const JsonParseCoordinator&) = delete;
        JsonParseCoordinator(JsonParseCoordinator&&) noexcept;
        JsonParseCoordinator& operator=(const JsonParseCoordinator&) = delete;
        JsonParseCoordinator& operator=(JsonParseCoordinator&&) noexcept;
        ~JsonParseCoordinator();

        [[nodiscard]] std::shared_ptr<ParseCoordinator> Clone() const override;

        void DeserializeIntoWrapper(std::istream* payload) override;

        [[nodiscard]] const IJsonParseHelper* GetHelper(RTTI::IdType helperTypeId) const;
        [[nodiscard]] bool IsContainingHelper(RTTI::IdType helperTypeId) const;
        [[nodiscard]] bool IsContainingHelper(const IJsonParseHelper* helper) const;
        bool PushBackHelper(std::unique_ptr<IJsonParseHelper> helper);
        bool PushFrontHelper(std::unique_ptr<IJsonParseHelper> helper);
        bool RemoveHelper(RTTI::IdType);

        [[nodiscard]] const IJsonValueTransmuter* GetTransmuter(RTTI::IdType transmuterTypeId) const;
        [[nodiscard]] bool IsContainingTransmuter(RTTI::IdType transmuterTypeId) const;
        [[nodiscard]] bool IsContainingTransmuter(const IJsonValueTransmuter* transmuter) const;
        bool PushBackTransmuter(std::unique_ptr<IJsonValueTransmuter> transmuter);
        bool RemoveTransmuter(RTTI::IdType);

    private:
        bool ParseRoot(const Json::Value& json);
        bool ParseObject(const Json::Value& json);
        bool ParseElement(const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index);

        bool PushHelper(std::unique_ptr<IJsonParseHelper> helper, bool isPushBack);

        SList<std::unique_ptr<IJsonParseHelper>> _helpers{};
        Vector<std::unique_ptr<IJsonValueTransmuter>> _transmuters{};

        friend JsonNestedObjectParseHelper;
        friend JsonKeyTokenParseHelper;

    };
}

#include "JsonParseCoordinator.inl"
