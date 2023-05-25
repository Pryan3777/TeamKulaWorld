#include "pch.h"
#include "CppUnitTest.h"
#include "Direction3D.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktopTests;
using namespace FieaGameEngine;

namespace LibraryDesktopTests {
    TEST_CLASS(Direction3DTests) {

    private:
        inline bool FloatsAreEquivalent(float lhs, float rhs) {
            return 0.001f > std::abs(lhs - rhs);
        }

        inline static _CrtMemState _startMemState;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&_startMemState);
    #endif
        }

        TEST_METHOD_CLEANUP(Cleanup) {
    #if defined(DEBUG) || defined(_DEBUG)
            _CrtMemState endMemState, diffMemState;
            _CrtMemCheckpoint(&endMemState);

            if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState)) {
                _CrtMemDumpStatistics(&diffMemState);
                Assert::Fail(L"Memory Leaks!");
            }
    #endif
        }

        TEST_METHOD(TestGettersAndSetters) {
            Direction3DInfo dir = static_cast<Direction3D>(5);

            Assert::AreEqual(Direction3D::Down, dir.GetDirection());

            Direction3DInfo current = Direction3D::Right;

            Assert::AreEqual(Direction3D::Right, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Right), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Right), current.GetDirectionAsInt());
            Assert::IsFalse(current.IsNegative());
            Assert::AreEqual(Direction3D::Right, current.GetAlignment());
            Assert::IsTrue(current.IsXAligned());
            Assert::IsFalse(current.IsYAligned());
            Assert::IsFalse(current.IsZAligned());

            current.SetDirection(Direction3D::Up);

            Assert::AreEqual(Direction3D::Up, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Up), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Up), current.GetDirectionAsInt());
            Assert::IsFalse(current.IsNegative());
            Assert::AreEqual(Direction3D::Up, current.GetAlignment());
            Assert::IsFalse(current.IsXAligned());
            Assert::IsTrue(current.IsYAligned());
            Assert::IsFalse(current.IsZAligned());

            current = Direction3D::Forward;

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Forward), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Forward), current.GetDirectionAsInt());
            Assert::IsFalse(current.IsNegative());
            Assert::AreEqual(Direction3D::Forward, current.GetAlignment());
            Assert::IsFalse(current.IsXAligned());
            Assert::IsFalse(current.IsYAligned());
            Assert::IsTrue(current.IsZAligned());

            current.SetDirection(Direction3D::Backward);

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Backward), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Backward), current.GetDirectionAsInt());
            Assert::IsTrue(current.IsNegative());
            Assert::AreEqual(Direction3D::Forward, current.GetAlignment());
            Assert::IsFalse(current.IsXAligned());
            Assert::IsFalse(current.IsYAligned());
            Assert::IsTrue(current.IsZAligned());

            current = Direction3D::Down;

            Assert::AreEqual(Direction3D::Down, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Down), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Down), current.GetDirectionAsInt());
            Assert::IsTrue(current.IsNegative());
            Assert::AreEqual(Direction3D::Up, current.GetAlignment());
            Assert::IsFalse(current.IsXAligned());
            Assert::IsTrue(current.IsYAligned());
            Assert::IsFalse(current.IsZAligned());

            current.SetDirection(Direction3D::Left);

            Assert::AreEqual(Direction3D::Left, current.GetDirection());
            Assert::AreEqual(static_cast<Direction3D_int_type>(Direction3D::Left), current.GetDirectionAsInt());
            Assert::AreEqual(static_cast<Direction3DInfo::int_type>(Direction3D::Left), current.GetDirectionAsInt());
            Assert::IsTrue(current.IsNegative());
            Assert::AreEqual(Direction3D::Right, current.GetAlignment());
            Assert::IsTrue(current.IsXAligned());
            Assert::IsFalse(current.IsYAligned());
            Assert::IsFalse(current.IsZAligned());
        }

        TEST_METHOD(TestConvertToIntVector3DAndFloatVector4D) {
            Direction3DInfo current = Direction3D::Right;

            auto rightIntVector = current.ToIntVector3D();

            Assert::AreEqual(1, rightIntVector.x());
            Assert::AreEqual(0, rightIntVector.y());
            Assert::AreEqual(0, rightIntVector.z());

            auto rightFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(1.f, rightFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(0.f, rightFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(0.f, rightFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, rightFloatVector.w));

            current.SetDirection(Direction3D::Up);

            auto upIntVector = current.ToIntVector3D();

            Assert::AreEqual(0, upIntVector.x());
            Assert::AreEqual(1, upIntVector.y());
            Assert::AreEqual(0, upIntVector.z());

            auto upFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(0.f, upFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(1.f, upFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(0.f, upFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, upFloatVector.w));

            current.SetDirection(Direction3D::Forward);

            auto forwardIntVector = current.ToIntVector3D();

            Assert::AreEqual(0, forwardIntVector.x());
            Assert::AreEqual(0, forwardIntVector.y());
            Assert::AreEqual(1, forwardIntVector.z());

            auto forwardFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(0.f, forwardFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(0.f, forwardFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(1.f, forwardFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, forwardFloatVector.w));

            current.SetDirection(Direction3D::Backward);

            auto backwardIntVector = current.ToIntVector3D();

            Assert::AreEqual(0, backwardIntVector.x());
            Assert::AreEqual(0, backwardIntVector.y());
            Assert::AreEqual(-1, backwardIntVector.z());

            auto backwardFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(0.f, backwardFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(0.f, backwardFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(-1.f, backwardFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, backwardFloatVector.w));

            current.SetDirection(Direction3D::Down);

            auto downIntVector = current.ToIntVector3D();

            Assert::AreEqual(0, downIntVector.x());
            Assert::AreEqual(-1, downIntVector.y());
            Assert::AreEqual(0, downIntVector.z());

            auto downFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(0.f, downFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(-1.f, downFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(0.f, downFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, downFloatVector.w));

            current.SetDirection(Direction3D::Left);

            auto leftIntVector = current.ToIntVector3D();

            Assert::AreEqual(-1, leftIntVector.x());
            Assert::AreEqual(0, leftIntVector.y());
            Assert::AreEqual(0, leftIntVector.z());

            auto leftFloatVector = current.ToFloatVector4D();

            Assert::IsTrue(FloatsAreEquivalent(-1.f, leftFloatVector.x));
            Assert::IsTrue(FloatsAreEquivalent(0.f, leftFloatVector.y));
            Assert::IsTrue(FloatsAreEquivalent(0.f, leftFloatVector.z));
            Assert::IsTrue(FloatsAreEquivalent(0.f, leftFloatVector.w));

        }

        TEST_METHOD(TestRotationAndInvert) {
            Direction3DInfo current = Direction3D::Right;

            current = current.RotateYawClockwise();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());

            current = current.RotateYawClockwise();

            Assert::AreEqual(Direction3D::Left, current.GetDirection());

            current = current.RotateYawClockwise();

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());

            current = current.RotateYawClockwise();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());

            current = current.RotateYawCounterclockwise();

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());

            current = current.RotateYawCounterclockwise();

            Assert::AreEqual(Direction3D::Left, current.GetDirection());

            current = current.RotateYawCounterclockwise();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());

            current = current.RotateYawCounterclockwise();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());

            current = current.RotateRollClockwise();

            Assert::AreEqual(Direction3D::Down, current.GetDirection());

            current = current.RotateRollClockwise();

            Assert::AreEqual(Direction3D::Left, current.GetDirection());

            current = current.RotateRollClockwise();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            current = current.RotateRollClockwise();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());

            current = current.RotateRollCounterclockwise();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            current = current.RotateRollCounterclockwise();

            Assert::AreEqual(Direction3D::Left, current.GetDirection());

            current = current.RotateRollCounterclockwise();

            Assert::AreEqual(Direction3D::Down, current.GetDirection());

            current = current.RotateRollCounterclockwise();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());

            current = current.RotateRollCounterclockwise();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            current = current.RotatePitchClockwise();

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());

            current = current.RotatePitchClockwise();

            Assert::AreEqual(Direction3D::Down, current.GetDirection());

            current = current.RotatePitchClockwise();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());

            current = current.RotatePitchClockwise();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            current = current.RotatePitchCounterclockwise();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());

            current = current.RotatePitchCounterclockwise();

            Assert::AreEqual(Direction3D::Down, current.GetDirection());

            current = current.RotatePitchCounterclockwise();

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());

            current = current.RotatePitchCounterclockwise();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            Assert::AreEqual(current, current.RotateYawClockwise());
            Assert::AreEqual(current, current.RotateYawCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Down, current.GetDirection());
            Assert::AreEqual(current, current.RotateYawClockwise());
            Assert::AreEqual(current, current.RotateYawCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Up, current.GetDirection());

            current = current.RotateRollClockwise();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());
            Assert::AreEqual(current, current.RotatePitchClockwise());
            Assert::AreEqual(current, current.RotatePitchCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Left, current.GetDirection());
            Assert::AreEqual(current, current.RotatePitchClockwise());
            Assert::AreEqual(current, current.RotatePitchCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Right, current.GetDirection());

            current = current.RotateYawClockwise();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());
            Assert::AreEqual(current, current.RotateRollClockwise());
            Assert::AreEqual(current, current.RotateRollCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Backward, current.GetDirection());
            Assert::AreEqual(current, current.RotateRollClockwise());
            Assert::AreEqual(current, current.RotateRollCounterclockwise());

            current = current.Invert();

            Assert::AreEqual(Direction3D::Forward, current.GetDirection());
        }

        TEST_METHOD(TestCross) {
            Assert::AreEqual(Direction3D::Forward, CrossProduct(Direction3D::Right, Direction3D::Up).GetDirection());
            Assert::AreEqual(Direction3D::Backward, CrossProduct(Direction3D::Up, Direction3D::Right).GetDirection());
            Assert::AreEqual(Direction3D::Right, CrossProduct(Direction3D::Up, Direction3D::Forward).GetDirection());
            Assert::AreEqual(Direction3D::Left, CrossProduct(Direction3D::Forward, Direction3D::Up).GetDirection());
            Assert::AreEqual(Direction3D::Up, CrossProduct(Direction3D::Forward, Direction3D::Right).GetDirection());
            Assert::AreEqual(Direction3D::Down, CrossProduct(Direction3D::Right, Direction3D::Forward).GetDirection());
            Assert::AreEqual(Direction3D::Backward, CrossProduct(Direction3D::Left, Direction3D::Up).GetDirection());
            Assert::AreEqual(Direction3D::Forward, CrossProduct(Direction3D::Up, Direction3D::Left).GetDirection());
            Assert::AreEqual(Direction3D::Left, CrossProduct(Direction3D::Down, Direction3D::Forward).GetDirection());
            Assert::AreEqual(Direction3D::Right, CrossProduct(Direction3D::Forward, Direction3D::Down).GetDirection());
            Assert::AreEqual(Direction3D::Down, CrossProduct(Direction3D::Backward, Direction3D::Right).GetDirection());
            Assert::AreEqual(Direction3D::Up, CrossProduct(Direction3D::Right, Direction3D::Backward).GetDirection());
            Assert::AreEqual(Direction3D::Forward, CrossProduct(Direction3D::Left, Direction3D::Down).GetDirection());
            Assert::AreEqual(Direction3D::Backward, CrossProduct(Direction3D::Down, Direction3D::Left).GetDirection());
            Assert::AreEqual(Direction3D::Right, CrossProduct(Direction3D::Down, Direction3D::Backward).GetDirection());
            Assert::AreEqual(Direction3D::Left, CrossProduct(Direction3D::Backward, Direction3D::Down).GetDirection());
            Assert::AreEqual(Direction3D::Up, CrossProduct(Direction3D::Backward, Direction3D::Left).GetDirection());
            Assert::AreEqual(Direction3D::Down, CrossProduct(Direction3D::Left, Direction3D::Backward).GetDirection());
            Assert::AreEqual(Direction3D::Backward, CrossProduct(Direction3D::Right, Direction3D::Down).GetDirection());
            Assert::AreEqual(Direction3D::Forward, CrossProduct(Direction3D::Down, Direction3D::Right).GetDirection());
            Assert::AreEqual(Direction3D::Left, CrossProduct(Direction3D::Up, Direction3D::Backward).GetDirection());
            Assert::AreEqual(Direction3D::Right, CrossProduct(Direction3D::Backward, Direction3D::Up).GetDirection());
            Assert::AreEqual(Direction3D::Down, CrossProduct(Direction3D::Forward, Direction3D::Left).GetDirection());
            Assert::AreEqual(Direction3D::Up, CrossProduct(Direction3D::Left, Direction3D::Forward).GetDirection());
        }
    };
}
