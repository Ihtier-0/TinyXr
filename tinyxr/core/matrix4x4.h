#ifndef TINYXR_CORE_MATRIX4X4_H
#define TINYXR_CORE_MATRIX4X4_H

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

class TINYXR_API Matrix4x4 {
public:
  Matrix4x4() = default;
  ~Matrix4x4() = default;

  // clang-format off
  Matrix4x4(float rc00, float rc01, float rc02, float rc03,
            float rc10, float rc11, float rc12, float rc13,
            float rc20, float rc21, float rc22, float rc23,
            float rc30, float rc31, float rc32, float rc33);
  // clang-format on

  float operator()(int r, int c) const;
  float& operator()(int r, int c);

  const float* data() const;

  static Matrix4x4 scale(float x, float y, float z);
  static Matrix4x4 translation(float x, float y, float z);

private:
  // clang-format off
  // row-major order
  float mData[4][4] = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1};
  // clang-format on
};

Matrix4x4 operator*(const Matrix4x4& lm, const Matrix4x4& rm);

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_MATRIX4X4_H
