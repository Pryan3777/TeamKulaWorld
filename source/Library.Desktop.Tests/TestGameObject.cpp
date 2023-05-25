#include "pch.h"
#include "TestGameObject.h"

using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    RTTI_DEFINITIONS(TestGameObject);

    const typename TestGameObject::key_type TestGameObject::UPDATE_COUNT_KEY = "UpdateCount"s;

    FieaGameEngine::SignatureVector TestGameObject::Signatures() {
        return FieaGameEngine::SignatureVector{ Signature{ UPDATE_COUNT_KEY, DatumType::Integer, true, size_type(1), offsetof(TestGameObject, _updateCount) } };
    }

    TestGameObject::TestGameObject() : TestGameObject{"Test"s} {}
}
