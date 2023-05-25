#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedSignatureRegistry.h"
#include "AttributedTestMonster.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace LibraryDesktopTests {
    TEST_CLASS(AttributedTests) {

    private:
        inline static _CrtMemState _startMemState;

        using size_type = Datum::size_type;
        using DatumType = FieaGameEngine::Datum::DatumType;
        using key_type = Attributed::key_type;

    public:
        TEST_METHOD_INITIALIZE(Initialize) {
            AttributedSignatureRegistry::RegisterSignatures<AttributedThing>();
            AttributedSignatureRegistry::RegisterSignatures<AttributedTestMonster, AttributedThing>();

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

            AttributedSignatureRegistry::UnregisterSignatures<AttributedTestMonster>();
            AttributedSignatureRegistry::UnregisterSignatures<AttributedThing>();
        }

        TEST_METHOD(BaseDefaultConstructor) {
            const key_type THIS_KEY = "this"s;
            const key_type LEVEL_KEY = "Level"s;
            const key_type MAXLEVEL_KEY = "MaxLevel"s;
            const key_type MAXHEALTH_KEY = "MaxHealth"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;
            const key_type TRANSFORM_KEY = "Transform"s;

            AttributedThing thing{};

            Assert::AreEqual(size_type(6), thing.PrescribedAttributeCount());
            Assert::AreEqual(thing.PrescribedAttributeCount(), thing.Size());
            Assert::IsTrue(thing.IsPrescribedAttribute(THIS_KEY));
            Assert::IsTrue(thing.IsPrescribedAttribute(LEVEL_KEY));
            Assert::IsTrue(thing.IsPrescribedAttribute(MAXLEVEL_KEY));
            Assert::IsTrue(thing.IsPrescribedAttribute(MAXHEALTH_KEY));
            Assert::IsTrue(thing.IsPrescribedAttribute(CURRENTHEALTH_KEY));
            Assert::IsTrue(thing.IsPrescribedAttribute(TRANSFORM_KEY));
            Assert::IsTrue(thing.IsAttribute(THIS_KEY));
            Assert::IsTrue(thing.IsAttribute(LEVEL_KEY));
            Assert::IsTrue(thing.IsAttribute(MAXLEVEL_KEY));
            Assert::IsTrue(thing.IsAttribute(MAXHEALTH_KEY));
            Assert::IsTrue(thing.IsAttribute(CURRENTHEALTH_KEY));
            Assert::IsTrue(thing.IsAttribute(TRANSFORM_KEY));
            Assert::IsTrue(&thing == thing[THIS_KEY].FrontPointer());
            Assert::AreSame(thing.Level(), thing[LEVEL_KEY].FrontInteger());
            Assert::AreSame(thing.MaxLevel(), thing[MAXLEVEL_KEY].FrontInteger());
            Assert::AreSame(thing.MaxHealth(), thing[MAXHEALTH_KEY].FrontFloat());
            Assert::AreSame(thing.CurrentHealth(), thing[CURRENTHEALTH_KEY].FrontFloat());
            Assert::AreSame(thing.Transform(), thing[TRANSFORM_KEY].FrontMatrix());
            Assert::IsTrue(thing[THIS_KEY].FrontPointer() == thing[size_type(0)].FrontPointer());
            Assert::AreSame(thing[LEVEL_KEY].FrontInteger(), thing[size_type(1)].FrontInteger());
            Assert::AreSame(thing[MAXLEVEL_KEY].FrontInteger(), thing[size_type(2)].FrontInteger());
            Assert::AreSame(thing[MAXHEALTH_KEY].FrontFloat(), thing[size_type(3)].FrontFloat());
            Assert::AreSame(thing[CURRENTHEALTH_KEY].FrontFloat(), thing[size_type(4)].FrontFloat());
            Assert::AreSame(thing[TRANSFORM_KEY].FrontMatrix(), thing[size_type(5)].FrontMatrix());

            int pointerCount = 0, intCount = 0, floatCount = 0, matrixCount = 0, otherCount = 0;

            thing.CForEachPrescribedAttribute(
                [&pointerCount, &intCount, &floatCount, &matrixCount, &otherCount](const key_type&, const Datum& datum) {
                    switch (datum.ActualType()) {

                    case DatumType::Integer:
                        ++intCount;
                        break;

                    case DatumType::Float:
                        ++floatCount;
                        break;

                    case DatumType::Matrix:
                        ++matrixCount;
                        break;

                    case DatumType::Pointer:
                        ++pointerCount;
                        break;

                    default:
                        ++otherCount;
                        break;

                    }

                    return false;
                }
            );

            Assert::AreEqual(1, pointerCount);
            Assert::AreEqual(2, intCount);
            Assert::AreEqual(2, floatCount);
            Assert::AreEqual(1, matrixCount);
            Assert::AreEqual(0, otherCount);

            thing.CForEachAuxiliaryAttribute([&otherCount](const key_type&, const Datum&){ ++otherCount; return false; });

            Assert::AreEqual(0, otherCount);

            auto size = thing.Size();
            thing.Clear();

            Assert::AreEqual(size, thing.Size());
        }

        TEST_METHOD(DerivedDefaultConstructor) {
            const key_type THIS_KEY = "this"s;
            const key_type LEVEL_KEY = "Level"s;
            const key_type MAXLEVEL_KEY = "MaxLevel"s;
            const key_type MAXHEALTH_KEY = "MaxHealth"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;
            const key_type TRANSFORM_KEY = "Transform"s;
            const key_type ENTRYMESSAGE_KEY = "EntryMessage"s;
            const key_type REWARD_KEY = "Reward"s;

            AttributedTestMonster monster{};

            Assert::AreEqual(size_type(8), monster.PrescribedAttributeCount());
            Assert::AreEqual(monster.PrescribedAttributeCount(), monster.Size());
            Assert::IsTrue(monster.IsPrescribedAttribute(THIS_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(LEVEL_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(MAXLEVEL_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(MAXHEALTH_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(CURRENTHEALTH_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(TRANSFORM_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(ENTRYMESSAGE_KEY));
            Assert::IsTrue(monster.IsPrescribedAttribute(REWARD_KEY));
            Assert::IsTrue(monster.IsAttribute(THIS_KEY));
            Assert::IsTrue(monster.IsAttribute(LEVEL_KEY));
            Assert::IsTrue(monster.IsAttribute(MAXLEVEL_KEY));
            Assert::IsTrue(monster.IsAttribute(MAXHEALTH_KEY));
            Assert::IsTrue(monster.IsAttribute(CURRENTHEALTH_KEY));
            Assert::IsTrue(monster.IsAttribute(TRANSFORM_KEY));
            Assert::IsTrue(monster.IsAttribute(ENTRYMESSAGE_KEY));
            Assert::IsTrue(monster.IsAttribute(REWARD_KEY));
            Assert::IsTrue(&monster == monster[THIS_KEY].FrontPointer());
            Assert::AreSame(monster.Level(), monster[LEVEL_KEY].FrontInteger());
            Assert::AreSame(monster.MaxLevel(), monster[MAXLEVEL_KEY].FrontInteger());
            Assert::AreSame(monster.MaxHealth(), monster[MAXHEALTH_KEY].FrontFloat());
            Assert::AreSame(monster.CurrentHealth(), monster[CURRENTHEALTH_KEY].FrontFloat());
            Assert::AreSame(monster.Transform(), monster[TRANSFORM_KEY].FrontMatrix());
            Assert::AreSame(monster.EntryMessage(), monster[ENTRYMESSAGE_KEY].FrontString());
            Assert::IsTrue(&(monster.Reward()) == &(monster[REWARD_KEY].FrontTable()));
            Assert::IsTrue(monster[REWARD_KEY].FrontTable().Is(Bar::TypeIdClass()));
            Assert::IsTrue(monster[THIS_KEY].FrontPointer() == monster[size_type(0)].FrontPointer());
            Assert::AreSame(monster[LEVEL_KEY].FrontInteger(), monster[size_type(1)].FrontInteger());
            Assert::AreSame(monster[MAXLEVEL_KEY].FrontInteger(), monster[size_type(2)].FrontInteger());
            Assert::AreSame(monster[MAXHEALTH_KEY].FrontFloat(), monster[size_type(3)].FrontFloat());
            Assert::AreSame(monster[CURRENTHEALTH_KEY].FrontFloat(), monster[size_type(4)].FrontFloat());
            Assert::AreSame(monster[TRANSFORM_KEY].FrontMatrix(), monster[size_type(5)].FrontMatrix());
            Assert::AreSame(monster[ENTRYMESSAGE_KEY].FrontString(), monster[size_type(6)].FrontString());
            Assert::AreSame(monster[REWARD_KEY].FrontTable(), monster[size_type(7)].FrontTable());

            int pointerCount = 0, intCount = 0, floatCount = 0, matrixCount = 0, stringCount = 0, tableCount = 0, otherCount = 0;

            monster.CForEachPrescribedAttribute
                ([&pointerCount, &intCount, &floatCount, &matrixCount, &stringCount, &tableCount, &otherCount]
                    (const key_type&, const Datum& datum) {
                        switch (datum.ActualType()) {

                        case DatumType::Integer:
                            ++intCount;
                            break;

                        case DatumType::Float:
                            ++floatCount;
                            break;

                        case DatumType::String:
                            ++stringCount;
                            break;

                        case DatumType::Matrix:
                            ++matrixCount;
                            break;

                        case DatumType::Pointer:
                            ++pointerCount;
                            break;

                        case DatumType::ExternalTable:
                            ++tableCount;
                            break;

                        default:
                            ++otherCount;
                            break;

                        }

                        return false;
                    }
            );

            Assert::AreEqual(1, pointerCount);
            Assert::AreEqual(2, intCount);
            Assert::AreEqual(2, floatCount);
            Assert::AreEqual(1, matrixCount);
            Assert::AreEqual(1, stringCount);
            Assert::AreEqual(1, tableCount);
            Assert::AreEqual(0, otherCount);

            monster.CForEachAuxiliaryAttribute([&otherCount](const key_type&, const Datum&){ ++otherCount; return false; });

            Assert::AreEqual(0, otherCount);

            auto size = monster.Size();
            monster.Clear();

            Assert::AreEqual(size, monster.Size());
        }

        TEST_METHOD(PrescribedIsExternalStorage) {
            using Integer = AttributedTestMonster::Integer;
            using Float = AttributedTestMonster::Float;

            const key_type LEVEL_KEY = "Level"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;

            AttributedTestMonster monster{};

            Integer level = monster.Level();

            Assert::AreEqual(level, monster[LEVEL_KEY].CFrontInteger());

            monster.LevelUp();

            Assert::AreNotEqual(level, monster[LEVEL_KEY].CFrontInteger());
            Assert::AreEqual(monster.Level(), monster[LEVEL_KEY].CFrontInteger());

            Float health = monster.CurrentHealth();

            Assert::AreEqual(health, monster[CURRENTHEALTH_KEY].CFrontFloat());

            monster.TakeDamage(50.f);

            Assert::AreNotEqual(health, monster[CURRENTHEALTH_KEY].CFrontFloat());
            Assert::AreEqual(monster.CurrentHealth(), monster[CURRENTHEALTH_KEY].CFrontFloat());

            health = monster.CurrentHealth();
            monster.Heal(25.f);

            Assert::AreNotEqual(health, monster[CURRENTHEALTH_KEY].CFrontFloat());
            Assert::AreEqual(monster.CurrentHealth(), monster[CURRENTHEALTH_KEY].CFrontFloat());
        }

        TEST_METHOD(CopySemantics) {
            const key_type THIS_KEY = "this"s;
            const key_type LEVEL_KEY = "Level"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;

            Datum::Matrix transform{
                {1.f, 2.f, 3.f, 4.f},
                {11.f, 12.f, 13.f, 14.f},
                {21.f, 22.f, 23.f, 24.f},
                {31.f, 32.f, 33.f, 34.f}
            };
            AttributedTestMonster monster{10, 15, 30.f, 25.f, transform, "Rawr!", 100};

            monster.AppendAuxiliaryAttribute("Attack"s) = 12.5f;
            monster.AppendAuxiliaryAttribute("Speed"s) = 1.f;
            Scope& projectile = monster.AppendAuxiliaryScopeAttribute("Projectile"s, size_type(2));
            projectile.Append("Attack"s) = 6.f;
            projectile.Append("Speed"s) = 5.f;

            AttributedTestMonster copy{monster};

            monster.CForEachAttribute([&copy, &THIS_KEY](const key_type& key, const Datum& datum) {
                Assert::IsTrue(copy.IsAttribute(key));
                if (key != THIS_KEY) {
                    Assert::AreEqual(datum, copy[key]);
                    Assert::AreNotSame(datum, copy[key]);
                }
                return false;
            });

            Assert::IsTrue(copy.IsAuxiliaryAttribute("Attack"s));
            Assert::IsTrue(copy.IsAuxiliaryAttribute("Speed"s));
            Assert::IsTrue(copy.IsAuxiliaryAttribute("Projectile"s));

            monster.LevelUp();
            monster.LevelUp();
            monster.TakeDamage(50.f);
            monster.SetReward(150);

            Assert::AreNotEqual(monster[LEVEL_KEY], copy[LEVEL_KEY]);
            Assert::AreNotEqual(monster[CURRENTHEALTH_KEY], copy[CURRENTHEALTH_KEY]);
            Assert::AreNotEqual(monster.Reward().Number(), copy.Reward().Number());

            monster.Clear();

            Assert::AreNotEqual(monster.Size(), copy.Size());

            copy = monster;

            monster.CForEachAttribute([&copy, &THIS_KEY](const key_type& key, const Datum& datum) {
                Assert::IsTrue(copy.IsAttribute(key));
                if (key != THIS_KEY) {
                    Assert::AreEqual(datum, copy[key]);
                    Assert::AreNotSame(datum, copy[key]);
                }
                return false;
            });
        }

        TEST_METHOD(MoveSemantics) {
            const key_type THIS_KEY = "this"s;
            const key_type LEVEL_KEY = "Level"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;

            Datum::Matrix transform{
                {1.f, 2.f, 3.f, 4.f},
                {11.f, 12.f, 13.f, 14.f},
                {21.f, 22.f, 23.f, 24.f},
                {31.f, 32.f, 33.f, 34.f}
            };
            AttributedTestMonster monster{10, 15, 30.f, 25.f, transform, "Rawr!", 100};

            monster.AppendAuxiliaryAttribute("Attack"s) = 12.5f;
            monster.AppendAuxiliaryAttribute("Speed"s) = 1.f;
            Scope& projectile = monster.AppendAuxiliaryScopeAttribute("Projectile"s, size_type(2));
            projectile.Append("Attack"s) = 6.f;
            projectile.Append("Speed"s) = 5.f;

            AttributedTestMonster source{monster};
            AttributedTestMonster copy{std::move(source)};

            monster.CForEachAttribute([&copy, &THIS_KEY](const key_type& key, const Datum& datum) {
                Assert::IsTrue(copy.IsAttribute(key));
                if (key != THIS_KEY) {
                    Assert::AreEqual(datum, copy[key]);
                    Assert::AreNotSame(datum, copy[key]);
                }
                return false;
            });

            Assert::IsTrue(copy.IsAuxiliaryAttribute("Attack"s));
            Assert::IsTrue(copy.IsAuxiliaryAttribute("Speed"s));
            Assert::IsTrue(copy.IsAuxiliaryAttribute("Projectile"s));

            monster.LevelUp();
            monster.LevelUp();
            monster.TakeDamage(50.f);
            monster.SetReward(150);

            Assert::AreNotEqual(monster[LEVEL_KEY], copy[LEVEL_KEY]);
            Assert::AreNotEqual(monster[CURRENTHEALTH_KEY], copy[CURRENTHEALTH_KEY]);
            Assert::AreNotEqual(monster.Reward().Number(), copy.Reward().Number());

            monster.Clear();

            Assert::AreNotEqual(monster.Size(), copy.Size());

            source = monster;
            copy = std::move(source);

            monster.CForEachAttribute([&copy, &THIS_KEY](const key_type& key, const Datum& datum) {
                Assert::IsTrue(copy.IsAttribute(key));
                if (key != THIS_KEY) {
                    Assert::AreEqual(datum, copy[key]);
                }
                return false;
            });
        }

        TEST_METHOD(Clone) {
            const key_type THIS_KEY = "this"s;

            Datum::Matrix transform{
                {1.f, 2.f, 3.f, 4.f},
                {11.f, 12.f, 13.f, 14.f},
                {21.f, 22.f, 23.f, 24.f},
                {31.f, 32.f, 33.f, 34.f}
            };
            AttributedTestMonster monster{10, 15, 30.f, 25.f, transform, "Rawr!", 100};

            monster.AppendAuxiliaryAttribute("Attack"s) = 12.5f;
            monster.AppendAuxiliaryAttribute("Speed"s) = 1.f;
            Scope& projectile = monster.AppendAuxiliaryScopeAttribute("Projectile"s, size_type(2));
            projectile.Append("Attack"s) = 6.f;
            projectile.Append("Speed"s) = 5.f;

            FieaGameEngine::Scope::ScopeUniquePointer clone = monster.Clone();
            AttributedTestMonster* copy = clone->As<AttributedTestMonster>();

            Assert::IsNotNull(copy);

            monster.CForEachAttribute([&copy, &THIS_KEY](const key_type& key, const Datum& datum) {
                Assert::IsTrue(copy->IsAttribute(key));
                if (key != THIS_KEY) {
                    Assert::AreEqual(datum, copy->operator[](key));
                    Assert::AreNotSame(datum, copy->operator[](key));
                }
                return false;
            });

            Assert::IsTrue(copy->IsAuxiliaryAttribute("Attack"s));
            Assert::IsTrue(copy->IsAuxiliaryAttribute("Speed"s));
            Assert::IsTrue(copy->IsAuxiliaryAttribute("Projectile"s));
        }

        TEST_METHOD(ForEachEdgeCases) {
            const key_type LEVEL_KEY = "Level"s;
            const key_type MAXLEVEL_KEY = "MaxLevel"s;
            const key_type MAXHEALTH_KEY = "MaxHealth"s;
            const key_type CURRENTHEALTH_KEY = "CurrentHealth"s;
            const key_type ATTACK_KEY = "Attack"s;
            const key_type SPEED_KEY = "Speed"s;

            Datum::Matrix transform{
                {1.f, 2.f, 3.f, 4.f},
                {11.f, 12.f, 13.f, 14.f},
                {21.f, 22.f, 23.f, 24.f},
                {31.f, 32.f, 33.f, 34.f}
            };
            AttributedTestMonster monster{10, 15, 30.f, 25.f, transform, "Rawr!", 100};
            AttributedTestMonster preauxMonster{monster};

            monster.AppendAuxiliaryAttribute(ATTACK_KEY) = 12.5f;
            monster.AppendAuxiliaryAttribute(SPEED_KEY) = 1.f;
            Scope& projectile = monster.AppendAuxiliaryScopeAttribute("Projectile"s, size_type(2));
            projectile.Append(ATTACK_KEY) = 6.f;
            projectile.Append(SPEED_KEY) = 5.f;

            Scope scope{};

            monster.ForEachPrescribedAttribute([&scope](const key_type& key, Datum& datum) {
                scope.Append(key) = datum;
                return false;
            });

            Assert::IsTrue(scope == preauxMonster);

            preauxMonster = monster;

            monster.ForEachAuxiliaryAttribute([](const key_type&, Datum& datum) {
                if (datum.ActualType() == DatumType::Integer) {
                    datum.FrontInteger() += 1;
                } else if (datum.ActualType() == DatumType::Float) {
                    datum.FrontFloat() += 1.f;
                }
                return false;
            });

            Assert::AreEqual(preauxMonster.Level(), monster[LEVEL_KEY].FrontInteger());
            Assert::AreEqual(preauxMonster.MaxLevel(), monster[MAXLEVEL_KEY].FrontInteger());
            Assert::AreEqual(preauxMonster.MaxHealth(), monster[MAXHEALTH_KEY].FrontFloat());
            Assert::AreEqual(preauxMonster.CurrentHealth(), monster[CURRENTHEALTH_KEY].FrontFloat());
            Assert::AreNotEqual(preauxMonster[ATTACK_KEY].FrontFloat(), monster[ATTACK_KEY].FrontFloat());
            Assert::IsTrue(preauxMonster[ATTACK_KEY].FrontFloat() < monster[ATTACK_KEY].FrontFloat());
            Assert::AreNotEqual(preauxMonster[SPEED_KEY].FrontFloat(), monster[SPEED_KEY].FrontFloat());
            Assert::IsTrue(preauxMonster[SPEED_KEY].FrontFloat() < monster[SPEED_KEY].FrontFloat());

            preauxMonster[ATTACK_KEY].FrontFloat() = monster[ATTACK_KEY].FrontFloat();
            preauxMonster[SPEED_KEY].FrontFloat() = monster[SPEED_KEY].FrontFloat();

            monster.ForEachAttribute([](const key_type&, Datum& datum) {
                if (datum.ActualType() == DatumType::Integer) {
                    datum.FrontInteger() += 1;
                } else if (datum.ActualType() == DatumType::Float) {
                    datum.FrontFloat() += 1.f;
                }
                return false;
            });

            monster.CForEachPrescribedAttribute([&preauxMonster](const key_type& key, const Datum& datum) {
                if (datum.ActualType() == DatumType::Integer) {
                    Assert::AreEqual(preauxMonster[key].CFrontInteger() + 1, datum.CFrontInteger());
                } else if (datum.ActualType() == DatumType::Float) {
                    Assert::AreEqual(preauxMonster[key].CFrontFloat() + 1.f, datum.CFrontFloat());
                }

                return false;
            });

            monster.CForEachAuxiliaryAttribute([&preauxMonster](const key_type& key, const Datum& datum) {
                if (datum.ActualType() == DatumType::Integer) {
                    Assert::AreEqual(preauxMonster[key].CFrontInteger() + 1, datum.CFrontInteger());
                } else if (datum.ActualType() == DatumType::Float) {
                    Assert::AreEqual(preauxMonster[key].CFrontFloat() + 1.f, datum.CFrontFloat());
                }

                return false;
            });
        }

        TEST_METHOD(AppendAuxiliaryEdgeCases) {
            const key_type THIS_KEY = "this"s;
            const key_type ENTRYMESSAGE_KEY = "EntryMessage"s;
            const key_type REWARD_KEY = "Reward"s;

            AttributedTestMonster monster{};

            Assert::ExpectException<std::invalid_argument>([&monster, &THIS_KEY](){
                monster.AppendAuxiliaryAttribute(THIS_KEY);
            });
            Assert::ExpectException<std::invalid_argument>([&monster, &THIS_KEY](){
                monster.AppendAuxiliaryScopeAttribute(THIS_KEY);
            });
            Assert::ExpectException<std::invalid_argument>([&monster, &ENTRYMESSAGE_KEY](){
                monster.AppendAuxiliaryAttribute(ENTRYMESSAGE_KEY);
            });
            Assert::ExpectException<std::invalid_argument>([&monster, &REWARD_KEY](){
                monster.AppendAuxiliaryScopeAttribute(REWARD_KEY);
            });
        }

        TEST_METHOD(ForEachEarlyTerminates) {
            Datum::Matrix transform{
                {1.f, 2.f, 3.f, 4.f},
                {11.f, 12.f, 13.f, 14.f},
                {21.f, 22.f, 23.f, 24.f},
                {31.f, 32.f, 33.f, 34.f}
            };
            AttributedTestMonster monster{10, 15, 30.f, 25.f, transform, "Rawr!", 100};

            monster.AppendAuxiliaryAttribute("Attack"s) = 12.5f;
            monster.AppendAuxiliaryAttribute("Speed"s) = 1.f;
            Scope& projectile = monster.AppendAuxiliaryScopeAttribute("Projectile"s, size_type(2));
            projectile.Append("Attack"s) = 6.f;
            projectile.Append("Speed"s) = 5.f;

            int counter = 0;

            monster.ForEachAttribute([&counter](const key_type&, Datum&){ ++counter; return true; });

            Assert::AreEqual(1, counter);

            monster.ForEachPrescribedAttribute([&counter](const key_type&, Datum&){ ++counter; return true; });

            Assert::AreEqual(2, counter);

            monster.ForEachAuxiliaryAttribute([&counter](const key_type&, Datum&){ ++counter; return true; });

            Assert::AreEqual(3, counter);

            monster.CForEachAttribute([&counter](const key_type&, const Datum&){ ++counter; return true; });

            Assert::AreEqual(4, counter);

            monster.CForEachPrescribedAttribute([&counter](const key_type&, const Datum&){ ++counter; return true; });

            Assert::AreEqual(5, counter);

            monster.CForEachAuxiliaryAttribute([&counter](const key_type&, const Datum&){ ++counter; return true; });

            Assert::AreEqual(6, counter);
        }
    };
}
