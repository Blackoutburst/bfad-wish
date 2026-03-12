#include <stdlib.h>

#include "math/math.hh"
#include "math/matrix.hh"

namespace Matrix {

    Matrix::It* create(U0) {
        Matrix::It* mat = (Matrix::It*)malloc(sizeof(Matrix::It));
        setIdentity(mat);

        return mat;
    }

    U0 destroy(Matrix::It* mat) {
        free(mat);
    }

    U0 setIdentity(Matrix::It* mat) {
        mat->m00 = 1.0f;
        mat->m01 = 0.0f;
        mat->m02 = 0.0f;
        mat->m03 = 0.0f;

        mat->m10 = 0.0f;
        mat->m11 = 1.0f;
        mat->m12 = 0.0f;
        mat->m13 = 0.0f;

        mat->m20 = 0.0f;
        mat->m21 = 0.0f;
        mat->m22 = 1.0f;
        mat->m23 = 0.0f;

        mat->m30 = 0.0f;
        mat->m31 = 0.0f;
        mat->m32 = 0.0f;
        mat->m33 = 1.0f;
    }

    F32* getValues(Matrix::It* mat) {
        F32* values = (F32*)malloc(sizeof(F32) * 16);
        values[M00] = mat->m00;
        values[M01] = mat->m01;
        values[M02] = mat->m02;
        values[M03] = mat->m03;
        
        values[M10] = mat->m10;
        values[M11] = mat->m11;
        values[M12] = mat->m12;
        values[M13] = mat->m13;

        values[M20] = mat->m20;
        values[M21] = mat->m21;
        values[M22] = mat->m22;
        values[M23] = mat->m23;

        values[M30] = mat->m30;
        values[M31] = mat->m31;
        values[M32] = mat->m32;
        values[M33] = mat->m33;

        return values;
    }

    F32 get(Matrix::It* mat, U32 index) {
        F32 values[] = {
            mat->m00, mat->m01, mat->m02, mat->m03,
            mat->m10, mat->m11, mat->m12, mat->m13,
            mat->m20, mat->m21, mat->m22, mat->m23,
            mat->m30, mat->m31, mat->m32, mat->m33,
        };

        return values[index];
    }

    U0 multiply(Matrix::It* result, Matrix::It* left, Matrix::It* right) {
        result->m00 = left->m00 * right->m00 + left->m01 * right->m10 + left->m02 * right->m20 + left->m03 * right->m30;
        result->m01 = left->m00 * right->m01 + left->m01 * right->m11 + left->m02 * right->m21 + left->m03 * right->m31;
        result->m02 = left->m00 * right->m02 + left->m01 * right->m12 + left->m02 * right->m22 + left->m03 * right->m32;
        result->m03 = left->m00 * right->m03 + left->m01 * right->m13 + left->m02 * right->m23 + left->m03 * right->m33;

        result->m10 = left->m10 * right->m00 + left->m11 * right->m10 + left->m12 * right->m20 + left->m13 * right->m30;
        result->m11 = left->m10 * right->m01 + left->m11 * right->m11 + left->m12 * right->m21 + left->m13 * right->m31;
        result->m12 = left->m10 * right->m02 + left->m11 * right->m12 + left->m12 * right->m22 + left->m13 * right->m32;
        result->m13 = left->m10 * right->m03 + left->m11 * right->m13 + left->m12 * right->m23 + left->m13 * right->m33;

        result->m20 = left->m20 * right->m00 + left->m21 * right->m10 + left->m22 * right->m20 + left->m23 * right->m30;
        result->m21 = left->m20 * right->m01 + left->m21 * right->m11 + left->m22 * right->m21 + left->m23 * right->m31;
        result->m22 = left->m20 * right->m02 + left->m21 * right->m12 + left->m22 * right->m22 + left->m23 * right->m32;
        result->m23 = left->m20 * right->m03 + left->m21 * right->m13 + left->m22 * right->m23 + left->m23 * right->m33;

        result->m30 = left->m30 * right->m00 + left->m31 * right->m10 + left->m32 * right->m20 + left->m33 * right->m30;
        result->m31 = left->m30 * right->m01 + left->m31 * right->m11 + left->m32 * right->m21 + left->m33 * right->m31;
        result->m32 = left->m30 * right->m02 + left->m31 * right->m12 + left->m32 * right->m22 + left->m33 * right->m32;
        result->m33 = left->m30 * right->m03 + left->m31 * right->m13 + left->m32 * right->m23 + left->m33 * right->m33;
    }


    U0 copy(Matrix::It* src, Matrix::It* dest) {
        dest->m00 = src->m00;
        dest->m01 = src->m01;
        dest->m02 = src->m02;
        dest->m03 = src->m03;

        dest->m10 = src->m10;
        dest->m11 = src->m11;
        dest->m12 = src->m12;
        dest->m13 = src->m13;

        dest->m20 = src->m20;
        dest->m21 = src->m21;
        dest->m22 = src->m22;
        dest->m23 = src->m23;

        dest->m30 = src->m30;
        dest->m31 = src->m31;
        dest->m32 = src->m32;
        dest->m33 = src->m33;
    }

    U0 ortho2D(Matrix::It* mat, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far) {
        F32 xOrt = 2.0f / (right - left);
        F32 yOrt = 2.0f / (top - bottom);
        F32 zOrt = -2.0f / (far - near);

        F32 tx = -(right + left) / (right - left);
        F32 ty = -(top + bottom) / (top - bottom);
        F32 tz = -(far + near) / (far - near);

        mat->m00 = xOrt;
        mat->m10 = 0.0f;
        mat->m20 = 0.0f;
        mat->m30 = tx;

        mat->m01 = 0.0f;
        mat->m11 = yOrt;
        mat->m21 = 0.0f;
        mat->m31 = ty;

        mat->m02 = 0.0f;
        mat->m12 = 0.0f;
        mat->m22 = zOrt;
        mat->m32 = tz;

        mat->m03 = 0.0f;
        mat->m13 = 0.0f;
        mat->m23 = 0.0f;
        mat->m33 = 1.0f;
    }

    U0 projection(Matrix::It* mat, F32 width, F32 height, F32 fov, F32 near, F32 far) {
        F32 aspectRatio = width / height;
        F32 yScale = 1.0f / tan(rad(fov / 2.0f));
        F32 xScale = yScale / aspectRatio;
        F32 frustumLength = far - near;

        mat->m00 = xScale;
        mat->m11 = yScale;
        mat->m22 = -((far + near) / frustumLength);
        mat->m23 = -1.0f;
        mat->m32 = -((2.0f * near * far) / frustumLength);
        mat->m33 = 0.0f;
    }

    U0 scale2d(Matrix::It* mat, F32 x, F32 y) {
        mat->m00 = mat->m00 * x;
        mat->m01 = mat->m01 * x;
        mat->m02 = mat->m02 * x;
        mat->m03 = mat->m03 * x;
        mat->m10 = mat->m10 * y;
        mat->m11 = mat->m11 * y;
        mat->m12 = mat->m12 * y;
        mat->m13 = mat->m13 * y;
    }

    U0 scale2dP(Matrix::It* mat, VectorF::It* vec) {
        mat->m00 = mat->m00 * vec->x;
        mat->m01 = mat->m01 * vec->x;
        mat->m02 = mat->m02 * vec->x;
        mat->m03 = mat->m03 * vec->x;
        mat->m10 = mat->m10 * vec->y;
        mat->m11 = mat->m11 * vec->y;
        mat->m12 = mat->m12 * vec->y;
        mat->m13 = mat->m13 * vec->y;
    }

    U0 scale3d(Matrix::It* mat, F32 x, F32 y, F32 z) {
        mat->m00 = mat->m00 * x;
        mat->m01 = mat->m01 * x;
        mat->m02 = mat->m02 * x;
        mat->m03 = mat->m03 * x;
        mat->m10 = mat->m10 * y;
        mat->m11 = mat->m11 * y;
        mat->m12 = mat->m12 * y;
        mat->m13 = mat->m13 * y;
        mat->m20 = mat->m20 * z;
        mat->m21 = mat->m21 * z;
        mat->m22 = mat->m22 * z;
        mat->m23 = mat->m23 * z;
    }

    U0 scale3dP(Matrix::It* mat, VectorF::It* vec) {
        mat->m00 = mat->m00 * vec->x;
        mat->m01 = mat->m01 * vec->x;
        mat->m02 = mat->m02 * vec->x;
        mat->m03 = mat->m03 * vec->x;
        mat->m10 = mat->m10 * vec->y;
        mat->m11 = mat->m11 * vec->y;
        mat->m12 = mat->m12 * vec->y;
        mat->m13 = mat->m13 * vec->y;
        mat->m20 = mat->m20 * vec->z;
        mat->m21 = mat->m21 * vec->z;
        mat->m22 = mat->m22 * vec->z;
        mat->m23 = mat->m23 * vec->z;
    }

    U0 tanslate2d(Matrix::It* mat, F32 x, F32 y) {
        Matrix::It* src = create();
        copy(mat, src);

        mat->m30 += src->m00 * x + src->m10 * y;
        mat->m31 += src->m01 * x + src->m11 * y;
        mat->m32 += src->m02 * x + src->m12 * y;
        mat->m33 += src->m03 * x + src->m13 * y;

        destroy(src);
    }

    U0 tanslate2dP(Matrix::It* mat, VectorF::It* vec) {
        Matrix::It* src = create();
        copy(mat, src);

        mat->m30 += src->m00 * vec->x + src->m10 * vec->y;
        mat->m31 += src->m01 * vec->x + src->m11 * vec->y;
        mat->m32 += src->m02 * vec->x + src->m12 * vec->y;
        mat->m33 += src->m03 * vec->x + src->m13 * vec->y;

        destroy(src);
    }

    U0 tanslate3d(Matrix::It* mat, F32 x, F32 y, F32 z) {
        Matrix::It* src = create();
        copy(mat, src);

        mat->m30 += src->m00 * x + src->m10 * y + src->m20 * z;
        mat->m31 += src->m01 * x + src->m11 * y + src->m21 * z;
        mat->m32 += src->m02 * x + src->m12 * y + src->m22 * z;
        mat->m33 += src->m03 * x + src->m13 * y + src->m23 * z;

        destroy(src);
    }

    U0 tanslate3dP(Matrix::It* mat, VectorF::It* vec) {
        Matrix::It* src = create();
        copy(mat, src);

        mat->m30 += src->m00 * vec->x + src->m10 * vec->y + src->m20 * vec->z;
        mat->m31 += src->m01 * vec->x + src->m11 * vec->y + src->m21 * vec->z;
        mat->m32 += src->m02 * vec->x + src->m12 * vec->y + src->m22 * vec->z;
        mat->m33 += src->m03 * vec->x + src->m13 * vec->y + src->m23 * vec->z;

        destroy(src);
    }

    U0 rotateP(Matrix::It* mat, F32 angle, VectorF::It* vec) {
        Matrix::It* src = create();
        copy(mat, src);

        F32 c = cos(angle);
        F32 s = sin(angle);
        F32 oneMinusC = 1.0f - c;
        F32 xy = vec->x * vec->y;
        F32 yz = vec->y * vec->z;
        F32 xz = vec->x * vec->z;
        F32 xs = vec->x * s;
        F32 ys = vec->y * s;
        F32 zs = vec->z * s;

        F32 f00 = vec->x * vec->x * oneMinusC + c;
        F32 f01 = xy * oneMinusC + zs;
        F32 f02 = xz * oneMinusC - ys;
        F32 f10 = xy * oneMinusC - zs;
        F32 f11 = vec->y * vec->y * oneMinusC + c;
        F32 f12 = yz * oneMinusC + xs;
        F32 f20 = xz * oneMinusC + ys;
        F32 f21 = yz * oneMinusC - xs;
        F32 f22 = vec->z * vec->z * oneMinusC + c;

        F32 t00 = src->m00 * f00 + src->m10 * f01 + src->m20 * f02;
        F32 t01 = src->m01 * f00 + src->m11 * f01 + src->m21 * f02;
        F32 t02 = src->m02 * f00 + src->m12 * f01 + src->m22 * f02;
        F32 t03 = src->m03 * f00 + src->m13 * f01 + src->m23 * f02;
        F32 t10 = src->m00 * f10 + src->m10 * f11 + src->m20 * f12;
        F32 t11 = src->m01 * f10 + src->m11 * f11 + src->m21 * f12;
        F32 t12 = src->m02 * f10 + src->m12 * f11 + src->m22 * f12;
        F32 t13 = src->m03 * f10 + src->m13 * f11 + src->m23 * f12;

        mat->m20 = src->m00 * f20 + src->m10 * f21 + src->m20 * f22;
        mat->m21 = src->m01 * f20 + src->m11 * f21 + src->m21 * f22;
        mat->m22 = src->m02 * f20 + src->m12 * f21 + src->m22 * f22;
        mat->m23 = src->m03 * f20 + src->m13 * f21 + src->m23 * f22;
        mat->m00 = t00;
        mat->m01 = t01;
        mat->m02 = t02;
        mat->m03 = t03;
        mat->m10 = t10;
        mat->m11 = t11;
        mat->m12 = t12;
        mat->m13 = t13;

        destroy(src);
    }

    U0 rotate(Matrix::It* mat, F32 angle, F32 x, F32 y, F32 z) {
        Matrix::It* src = create();
        copy(mat, src);

        F32 c = cos(angle);
        F32 s = sin(angle);
        F32 oneMinusC = 1.0f - c;
        F32 xy = x * y;
        F32 yz = y * z;
        F32 xz = x * z;
        F32 xs = x * s;
        F32 ys = y * s;
        F32 zs = z * s;

        F32 f00 = x * x * oneMinusC + c;
        F32 f01 = xy * oneMinusC + zs;
        F32 f02 = xz * oneMinusC - ys;
        F32 f10 = xy * oneMinusC - zs;
        F32 f11 = y * y * oneMinusC + c;
        F32 f12 = yz * oneMinusC + xs;
        F32 f20 = xz * oneMinusC + ys;
        F32 f21 = yz * oneMinusC - xs;
        F32 f22 = z * z * oneMinusC + c;

        F32 t00 = src->m00 * f00 + src->m10 * f01 + src->m20 * f02;
        F32 t01 = src->m01 * f00 + src->m11 * f01 + src->m21 * f02;
        F32 t02 = src->m02 * f00 + src->m12 * f01 + src->m22 * f02;
        F32 t03 = src->m03 * f00 + src->m13 * f01 + src->m23 * f02;
        F32 t10 = src->m00 * f10 + src->m10 * f11 + src->m20 * f12;
        F32 t11 = src->m01 * f10 + src->m11 * f11 + src->m21 * f12;
        F32 t12 = src->m02 * f10 + src->m12 * f11 + src->m22 * f12;
        F32 t13 = src->m03 * f10 + src->m13 * f11 + src->m23 * f12;

        mat->m20 = src->m00 * f20 + src->m10 * f21 + src->m20 * f22;
        mat->m21 = src->m01 * f20 + src->m11 * f21 + src->m21 * f22;
        mat->m22 = src->m02 * f20 + src->m12 * f21 + src->m22 * f22;
        mat->m23 = src->m03 * f20 + src->m13 * f21 + src->m23 * f22;
        mat->m00 = t00;
        mat->m01 = t01;
        mat->m02 = t02;
        mat->m03 = t03;
        mat->m10 = t10;
        mat->m11 = t11;
        mat->m12 = t12;
        mat->m13 = t13;

        destroy(src);
    }
}
