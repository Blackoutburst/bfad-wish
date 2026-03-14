#pragma once

#include "engine/types.hh"
#include "engine/math/vectorf.hh"

#define M00 0x00
#define M01 0x01
#define M02 0x02
#define M03 0x03

#define M10 0x04
#define M11 0x05
#define M12 0x06
#define M13 0x07

#define M20 0x08
#define M21 0x09
#define M22 0x0A
#define M23 0x0B

#define M30 0x0C
#define M31 0x0D
#define M32 0x0E
#define M33 0x0F

namespace Matrix {

    struct It {
        F32 m00;
        F32 m01;
        F32 m02;
        F32 m03;
        
        F32 m10;
        F32 m11;
        F32 m12;
        F32 m13;
        
        F32 m20;
        F32 m21;
        F32 m22;
        F32 m23;
        
        F32 m30;
        F32 m31;
        F32 m32;
        F32 m33;
    };

    Matrix::It* create(U0);
    U0 destroy(Matrix::It* mat);
    F32* getValues(Matrix::It* mat);
    U0 writeTo(Matrix::It* mat, F32* dest);
    U0 setIdentity(Matrix::It* mat);
    F32 get(Matrix::It* mat, U32 index);
    U0 multiply(Matrix::It* result, Matrix::It* left, Matrix::It* right);
    U0 copy(Matrix::It* src, Matrix::It* dest);
    U0 ortho2D(Matrix::It* mat, F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far);
    U0 projection(Matrix::It* mat, F32 width, F32 height, F32 fov, F32 near, F32 far);
    U0 scale2d(Matrix::It* mat, F32 x, F32 y);
    U0 scale2dP(Matrix::It* mat, VectorF::It* vec);
    U0 scale3d(Matrix::It* mat, F32 x, F32 y, F32 z);
    U0 scale3dP(Matrix::It* mat, VectorF::It* vec);
    U0 translate2d(Matrix::It* mat, F32 x, F32 y);
    U0 translate2dP(Matrix::It* mat, VectorF::It* vec);
    U0 translate3d(Matrix::It* mat, F32 x, F32 y, F32 z);
    U0 translate3dP(Matrix::It* mat, VectorF::It* vec);
    U0 rotate(Matrix::It* mat, F32 angle, F32 x, F32 y, F32 z);
    U0 rotateP(Matrix::It* mat, F32 angle, VectorF::It* vec);
}
