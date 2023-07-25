#include "tinyxr/core/matrix4x4.h"

TINYXR_NAMESPACE_OPEN

// clang-format off
Matrix4x4::Matrix4x4(float rc00, float rc01, float rc02, float rc03,
                     float rc10, float rc11, float rc12, float rc13,
                     float rc20, float rc21, float rc22, float rc23,
                     float rc30, float rc31, float rc32, float rc33)
// clang-format on
{
  mData[0][0] = rc00;
  mData[0][1] = rc01;
  mData[0][2] = rc02;
  mData[0][3] = rc03;

  mData[1][0] = rc10;
  mData[1][1] = rc11;
  mData[1][2] = rc12;
  mData[1][3] = rc13;

  mData[2][0] = rc20;
  mData[2][1] = rc21;
  mData[2][2] = rc22;
  mData[2][3] = rc23;

  mData[3][0] = rc30;
  mData[3][1] = rc31;
  mData[3][2] = rc32;
  mData[3][3] = rc33;
}

float Matrix4x4::operator()(int r, int c) const { return mData[r][c]; }

float& Matrix4x4::operator()(int r, int c) { return mData[r][c]; }

const float* Matrix4x4::data() const { return &(mData[0][0]); }

Matrix4x4 Matrix4x4::scale(float x, float y, float z) {
  Matrix4x4 scale;
  scale(0, 0) = x;
  scale(1, 1) = y;
  scale(2, 2) = z;
  return scale;
}

Matrix4x4 Matrix4x4::translation(float x, float y, float z) {
  Matrix4x4 translation;
  translation(0, 3) = x;
  translation(1, 3) = y;
  translation(2, 3) = z;
  return translation;
}

Matrix4x4 operator*(const Matrix4x4& lm, const Matrix4x4& rm) {
  Matrix4x4 out;

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      float v = 0;
      for (int k = 0; k < 4; ++k) {
        v += lm(r, k) * rm(k, c);
      }
      out(r, c) = v;
    }
  }

  return out;
}

TINYXR_NAMESPACE_CLOSE
