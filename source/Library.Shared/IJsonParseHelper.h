#pragma once
#include <memory>
#include <json/json.h>
#include "JsonParseCoordinator.h"
#include "RTTI.h"

namespace FieaGameEngine {
    /// <summary>
    /// Abstract interface for helpers to the JSON parse coordinator.
    /// </summary>
    class IJsonParseHelper : public RTTI {
        RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

    protected:
        using Wrapper = ParseCoordinator::Wrapper;

    public:
        virtual ~IJsonParseHelper() = default;

        /// <summary>
        /// Initializes the helper. Classes implementing this interface should override this if they need any special initialization.
        /// </summary>
        virtual inline void Initialize() {}

        /// <summary>
        /// Deinitializes the helper. Classes implementing this interface should override this if they need any special deinitialization.
        /// </summary>
        virtual inline void Cleanup() {}

        /// <returns>Is this helper capable of parsing this JSON into this wrapper?</returns>
        [[nodiscard]] virtual bool IsAbleToParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) = 0;

        /// <summary>
        /// If this helper is capable of parsing this JSON into this wrapper, attempts to parse the JSON into the wrapper.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        virtual bool TryParse(Wrapper& wrapper, const Json::String& key, const Json::Value& value, bool isArrayElement, Json::ArrayIndex index) = 0;

        /// <summary>
        /// Conducts any cleanup that may be necessary for parsing.
        /// </summary>
        virtual bool EndParse(Wrapper& wrapper, const Json::String& key) = 0;

        /// <returns>Creates a newly initialized copy of this object. The helpers will share configuration but not state.</returns>
        [[nodiscard]] virtual std::unique_ptr<IJsonParseHelper> Create() const = 0;

    };
}

#include "IJsonParseHelper.inl"
