#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedSignatureRegistry.h"
#include "GameplayState.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_MODULE_INITIALIZE(ModuleInitialize) {
        GameplayState::CreateSingleton();
        AttributedSignatureRegistry::CreateSingleton();
    }

    TEST_MODULE_CLEANUP(ModuleCleanup) {
        AttributedSignatureRegistry::DestroySingleton();
        GameplayState::DestroySingleton();
    }
}
