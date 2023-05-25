#pragma once
#include "IntVector3D.h"

namespace FieaGameEngine {
    inline IntVector3D::IntVector3D(int_type x, int_type y, int_type z) : _x{x}, _y{y}, _z{z} {}

    inline bool IntVector3D::operator==(const IntVector3D& other) const { return std::memcmp(this, &other, sizeof(IntVector3D)) == 0; }
    inline bool IntVector3D::operator!=(const IntVector3D& other) const { return !operator==(other); }
    inline IntVector3D IntVector3D::operator+(const IntVector3D& other) const { return IntVector3D{_x + other._x, _y + other._y, _z + other._z}; }
    inline IntVector3D IntVector3D::operator-(const IntVector3D& other) const { return IntVector3D{_x - other._x, _y - other._y, _z - other._z}; }
    inline IntVector3D IntVector3D::operator*(int_type a) const { return IntVector3D{_x * a, _y * a, _z * a}; }
    inline FloatVector4D IntVector3D::operator*(FieaGameEngine::Datum::Float a) const { return FloatVector4D{_x * a, _y * a, _z * a, 0.f}; }

    inline typename IntVector3D::int_type IntVector3D::x() const { return _x; }
    inline typename IntVector3D::int_type IntVector3D::y() const { return _y; }
    inline typename IntVector3D::int_type IntVector3D::z() const { return _z; }

    inline FloatVector4D IntVector3D::ToFloatVector4D() const {
        using f_t = FieaGameEngine::Datum::Float;
        return FloatVector4D{static_cast<f_t>(_x), static_cast<f_t>(_y), static_cast<f_t>(_z), 0.f};
    }
    inline IntVector3D::operator FloatVector4D() const { return ToFloatVector4D(); }
}
