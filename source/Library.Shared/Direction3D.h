#pragma once
#include <cstdint>
#include <DirectXMath.h>
#include "IntVector3D.h"

namespace FieaGameEngine {
    using Direction3D_int_type = std::uint8_t;

    enum class Direction3D : Direction3D_int_type {
        Right = Direction3D_int_type(0),
        Up = Direction3D_int_type(1),
        Forward = Direction3D_int_type(2),
        Left = Direction3D_int_type(4),
        Down = Direction3D_int_type(5),
        Backward = Direction3D_int_type(6)
    };

    class Direction3DInfo final {

    public:
        using int_type = Direction3D_int_type;

        Direction3DInfo(Direction3D);

        Direction3DInfo& operator=(Direction3D);

        [[nodiscard]] bool operator==(const Direction3DInfo&) const;
        [[nodiscard]] bool operator!=(const Direction3DInfo&) const;

        [[nodiscard]] int_type GetDirectionAsInt() const;
        [[nodiscard]] Direction3D GetDirection() const;
        [[nodiscard]] void SetDirection(Direction3D);

        [[nodiscard]] Direction3D GetAlignment() const;
        [[nodiscard]] Direction3DInfo GetAlignmentInfo() const;

        [[nodiscard]] bool IsXAligned() const;
        [[nodiscard]] int_type IsYAligned() const;
        [[nodiscard]] int_type IsZAligned() const;
        [[nodiscard]] int_type IsNegative() const;

        [[nodiscard]] Direction3DInfo Invert() const;
        [[nodiscard]] Direction3DInfo Cross(const Direction3DInfo&) const;

        [[nodiscard]] Direction3DInfo RotatePitchCounterclockwise() const;
        [[nodiscard]] Direction3DInfo RotateRollCounterclockwise() const;
        [[nodiscard]] Direction3DInfo RotateYawCounterclockwise() const;
        [[nodiscard]] Direction3DInfo RotatePitchClockwise() const;
        [[nodiscard]] Direction3DInfo RotateRollClockwise() const;
        [[nodiscard]] Direction3DInfo RotateYawClockwise() const;

        [[nodiscard]] static IntVector3D Direction3DToIntVector3D(Direction3D direction, bool forcePositive = false);
        [[nodiscard]] static FloatVector4D Direction3DToFloatVector4D(Direction3D direction, bool forcePositive = false);
        [[nodiscard]] static DirectX::XMFLOAT3 Direction3DToXMFLOAT3(Direction3D direction, bool forcePositive = false);

        [[nodiscard]] IntVector3D ToIntVector3D(bool forcePositive = false) const;
        [[nodiscard]] explicit operator IntVector3D() const;
        [[nodiscard]] FloatVector4D ToFloatVector4D(bool forcePositive = false) const;
        [[nodiscard]] explicit operator FloatVector4D() const;
        [[nodiscard]] DirectX::XMFLOAT3 ToXMFLOAT3(bool forcePositive = false) const;
        [[nodiscard]] explicit operator DirectX::XMFLOAT3() const;

    private:
        using signed_type = std::int8_t;

        enum Direction3DBits : int_type {
            BIT_IS_NEGATIVE = int_type(4),
            BIT_Z_ALIGNED = int_type(2),
            BIT_Y_ALIGNED = int_type(1),
            ALIGNMENT_BITS = int_type(3),
            ALL_BITS = int_type(7),
            DIR3D_MAXVALUE = int_type(8)
        };

        int_type _direction;

        [[nodiscard]] int_type GetAlignmentAsInt() const;
        [[nodiscard]] bool IsEven() const;

        [[nodiscard]] int_type GetBitYAligned() const;
        [[nodiscard]] int_type GetBitZAligned() const;
        [[nodiscard]] int_type GetBitIsNegative() const;

        [[nodiscard]] bool IsNext(int_type unit) const;

        [[nodiscard]] Direction3D RotatePitch(bool isCounterclockwise) const;
        [[nodiscard]] Direction3D RotateRoll(bool isCounterclockwise) const;
        [[nodiscard]] Direction3D RotateYaw(bool isCounterclockwise) const;

    };

    [[nodiscard]] Direction3DInfo CrossProduct(const Direction3DInfo& p, const Direction3DInfo& q);
}

#include "Direction3D.inl"
