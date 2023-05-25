#pragma once
#include "Datum.h"
#include "Scope.h"
#include <DirectXMath.h>
#include <gsl/gsl>

namespace FieaGameEngine {
    using FloatVector4D = Datum::Vector;

    class IntVector3D final {

    public:
        using int_type = Datum::Integer;

        IntVector3D() {};
        IntVector3D(int_type x, int_type y, int_type z);

        [[nodiscard]] bool operator==(const IntVector3D&) const;
        [[nodiscard]] bool operator!=(const IntVector3D&) const;
        [[nodiscard]] IntVector3D operator+(const IntVector3D&) const;
        [[nodiscard]] IntVector3D operator-(const IntVector3D&) const;
        [[nodiscard]] IntVector3D operator*(int_type) const;
        [[nodiscard]] FloatVector4D operator*(Datum::Float) const;

        [[nodiscard]] int_type x() const;
        [[nodiscard]] int_type y() const;
        [[nodiscard]] int_type z() const;

        [[nodiscard]] FloatVector4D ToFloatVector4D() const;
        [[nodiscard]] explicit operator FloatVector4D() const;

        inline friend FloatVector4D operator+(const IntVector3D& i, const FloatVector4D& f) {
            return FloatVector4D{i._x + f.x, i._y + f.y, i._z + f.z, f.w};
        }
        inline friend FloatVector4D operator+(const FloatVector4D& f, const IntVector3D& i) {
            return FloatVector4D{f.x + i._x, f.y + i._y, f.z + i._z, f.w};
        }
        inline friend FloatVector4D operator-(const IntVector3D& i, const FloatVector4D& f) {
            return FloatVector4D{i._x - f.x, i._y - f.y, i._z - f.z, f.w};
        }
        inline friend FloatVector4D operator-(const FloatVector4D& f, const IntVector3D& i) {
            return FloatVector4D{f.x - i._x, f.y - i._y, f.z - i._z, f.w};
        }

        inline DirectX::XMFLOAT3 ConvertToXMFLOAT3()
        {
            return DirectX::XMFLOAT3(gsl::narrow<float>(_x), gsl::narrow<float>(_y), gsl::narrow<float>(_z));
        }

    private:
        int_type _x{0};
        int_type _y{0};
        int_type _z{0};

    };
}

#include "IntVector3D.inl"
