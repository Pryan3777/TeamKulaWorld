#pragma once
#include "Direction3D.h"

namespace FieaGameEngine {
    inline Direction3DInfo::Direction3DInfo(Direction3D value) : _direction{static_cast<int_type>(value)} {}

    inline Direction3DInfo& Direction3DInfo::operator=(Direction3D value) { SetDirection(value); return *this; }

    inline bool Direction3DInfo::operator==(const Direction3DInfo& other) const { return _direction == other._direction; }
    inline bool Direction3DInfo::operator!=(const Direction3DInfo& other) const { return !operator==(other); }

    inline typename Direction3DInfo::int_type Direction3DInfo::GetDirectionAsInt() const { return _direction; }
    inline Direction3D Direction3DInfo::GetDirection() const { return static_cast<Direction3D>(_direction); }
    inline void Direction3DInfo::SetDirection(Direction3D value) { _direction = static_cast<int_type>(value); }

    inline bool Direction3DInfo::IsXAligned() const { return ALIGNMENT_BITS == (GetAlignmentAsInt() ^ ALIGNMENT_BITS); }
    inline typename Direction3DInfo::int_type Direction3DInfo::IsYAligned() const { return GetBitYAligned(); }
    inline typename Direction3DInfo::int_type Direction3DInfo::IsZAligned() const { return GetBitZAligned(); }
    inline typename Direction3DInfo::int_type Direction3DInfo::IsNegative() const { return GetBitIsNegative(); }
    inline typename Direction3DInfo::int_type Direction3DInfo::GetBitYAligned() const { return _direction & BIT_Y_ALIGNED; }
    inline typename Direction3DInfo::int_type Direction3DInfo::GetBitZAligned() const { return _direction & BIT_Z_ALIGNED; }
    inline typename Direction3DInfo::int_type Direction3DInfo::GetBitIsNegative() const { return _direction & BIT_IS_NEGATIVE; }

    inline Direction3DInfo Direction3DInfo::GetAlignmentInfo() const { return Direction3DInfo(GetAlignment()); }
    inline Direction3D Direction3DInfo::GetAlignment() const { return static_cast<Direction3D>(GetAlignmentAsInt()); }
    inline typename Direction3DInfo::int_type Direction3DInfo::GetAlignmentAsInt() const { return _direction & ALIGNMENT_BITS; }

    inline bool Direction3DInfo::IsEven() const { return (_direction % int_type(2)) == int_type(0); }

    inline Direction3DInfo Direction3DInfo::Invert() const { return static_cast<Direction3D>((_direction + BIT_IS_NEGATIVE) % DIR3D_MAXVALUE); }

    inline Direction3DInfo Direction3DInfo::Cross(const Direction3DInfo& other) const {
        int_type alignment = ALIGNMENT_BITS ^ (_direction | other._direction) & ALIGNMENT_BITS;
        int_type sign = GetBitIsNegative() ^ other.GetBitIsNegative() ^ (IsNext(other.GetAlignmentAsInt()) ? int_type(0) : BIT_IS_NEGATIVE);
        return static_cast<Direction3D>((GetAlignmentAsInt() == other.GetAlignmentAsInt()) ? _direction : (alignment | sign));
    }

    inline Direction3DInfo CrossProduct(const Direction3DInfo& p, const Direction3DInfo& q) { return p.Cross(q); }

    inline bool Direction3DInfo::IsNext(int_type align) const { return (GetAlignmentAsInt() + int_type(1)) == (align == int_type(0) ? ALIGNMENT_BITS : align); }

    inline Direction3DInfo Direction3DInfo::RotatePitchCounterclockwise() const { return RotatePitch(true); }
    inline Direction3DInfo Direction3DInfo::RotateRollCounterclockwise() const { return RotateRoll(true); }
    inline Direction3DInfo Direction3DInfo::RotateYawCounterclockwise() const { return RotateYaw(true); }
    inline Direction3DInfo Direction3DInfo::RotatePitchClockwise() const { return RotatePitch(false); }
    inline Direction3DInfo Direction3DInfo::RotateRollClockwise() const { return RotateRoll(false); }
    inline Direction3DInfo Direction3DInfo::RotateYawClockwise() const { return RotateYaw(false); }

    inline Direction3D Direction3DInfo::RotatePitch(bool isCounterclockwise) const {
        signed_type delta = isCounterclockwise ? (IsEven() ? signed_type(3) : signed_type(1)) : (IsEven() ? signed_type(-1) : signed_type(-3));
        int_type result = (DIR3D_MAXVALUE + _direction + delta) % DIR3D_MAXVALUE;
        return static_cast<Direction3D>((ALIGNMENT_BITS & result ^ ALIGNMENT_BITS) ? result : _direction);
    }
    inline Direction3D Direction3DInfo::RotateRoll(bool isCounterclockwise) const {
        signed_type delta = isCounterclockwise ? (IsEven() ? signed_type(1) : signed_type(3)) : (IsEven() ? signed_type(-3) : signed_type(-1));
        int_type result = (DIR3D_MAXVALUE + _direction + delta) % DIR3D_MAXVALUE;
        return static_cast<Direction3D>((ALIGNMENT_BITS & result ^ ALIGNMENT_BITS) ? result : _direction);
    }
    inline Direction3D Direction3DInfo::RotateYaw(bool isCounterclockwise) const {
        signed_type delta = isCounterclockwise ? signed_type(-2) : signed_type(2);
        int_type result = (DIR3D_MAXVALUE + _direction + delta) % DIR3D_MAXVALUE;
        return static_cast<Direction3D>((ALIGNMENT_BITS & result ^ ALIGNMENT_BITS) ? result : _direction);
    }

    inline IntVector3D Direction3DInfo::Direction3DToIntVector3D(Direction3D direction, bool forcePositive) {
        auto d = static_cast<Direction3D_int_type>(direction);
        return IntVector3D{
            (d & Direction3DInfo::ALIGNMENT_BITS) ? 0 : ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1 : 1),
            (d & Direction3DInfo::BIT_Y_ALIGNED) ? ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1 : 1) : 0,
            (d & Direction3DInfo::BIT_Z_ALIGNED) ? ((forcePositive || (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? 1 : -1) : 0
        };
    }
    inline FloatVector4D Direction3DInfo::Direction3DToFloatVector4D(Direction3D direction, bool forcePositive) {
        auto d = static_cast<Direction3D_int_type>(direction);
        return FloatVector4D{
            (d & Direction3DInfo::ALIGNMENT_BITS) ? 0 : ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1 : 1),
            (d & Direction3DInfo::BIT_Y_ALIGNED) ? ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1 : 1) : 0,
            (d & Direction3DInfo::BIT_Z_ALIGNED) ? ((forcePositive || (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? 1 : -1) : 0,
            0.f
        };
    }
    inline DirectX::XMFLOAT3 Direction3DInfo::Direction3DToXMFLOAT3(Direction3D direction, bool forcePositive) {
        auto d = static_cast<Direction3D_int_type>(direction);
        return DirectX::XMFLOAT3{
            (d & Direction3DInfo::ALIGNMENT_BITS) ? 0.f : ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1.f : 1.f),
            (d & Direction3DInfo::BIT_Y_ALIGNED) ? ((!forcePositive && (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? -1.f : 1.f) : 0.f,
            (d & Direction3DInfo::BIT_Z_ALIGNED) ? ((forcePositive || (d & Direction3DInfo::BIT_IS_NEGATIVE)) ? 1.f : -1.f) : 0.f
        };
    }
    inline IntVector3D Direction3DInfo::ToIntVector3D(bool forcePositive) const { return Direction3DToIntVector3D(static_cast<Direction3D>(_direction), forcePositive); }
    inline Direction3DInfo::operator IntVector3D() const { return ToIntVector3D(); }
    inline FloatVector4D Direction3DInfo::ToFloatVector4D(bool forcePositive) const { return Direction3DToFloatVector4D(static_cast<Direction3D>(_direction), forcePositive); }
    inline Direction3DInfo::operator FloatVector4D() const { return ToFloatVector4D(); }
    inline DirectX::XMFLOAT3 Direction3DInfo::ToXMFLOAT3(bool forcePositive) const { return Direction3DToXMFLOAT3(static_cast<Direction3D>(_direction), forcePositive); }
    inline Direction3DInfo::operator DirectX::XMFLOAT3() const { return ToXMFLOAT3(); }
}
