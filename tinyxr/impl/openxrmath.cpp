#include "tinyxr/impl/openxrmath.h"

#include <cmath>

TINYXR_NAMESPACE_OPEN

XrPosef XrPosefIdentity() {
  XrPosef pose = {};
  pose.orientation.w = 1;
  return pose;
}

Matrix4x4 createProjection(const XrFovf& fov, const float nearZ,
                           const float farZ) {
  const float tanLeft = std::tanf(fov.angleLeft);
  const float tanRight = std::tanf(fov.angleRight);

  const float tanDown = std::tanf(fov.angleDown);
  const float tanUp = std::tanf(fov.angleUp);

  const float tanAngleWidth = tanRight - tanLeft;
  const float tanAngleHeight = tanUp - tanDown;

  const float offsetZ = nearZ;

  if (farZ <= nearZ) {
    // clang-format off
    return Matrix4x4(2.0f / tanAngleWidth,                  0.0f, (tanRight + tanLeft) / tanAngleWidth ,               0.0f,
                                     0.0f, 2.0f / tanAngleHeight,    (tanUp + tanDown) / tanAngleHeight,               0.0f,
                                     0.0f,                  0.0f,                                 -1.0f, -(nearZ + offsetZ),
                                     0.0f,                  0.0f,                                 -1.0f,               0.0f);
    // clang-format on
  } else {
    // clang-format off
    return Matrix4x4(2.0f / tanAngleWidth,                  0.0f, (tanRight + tanLeft) / tanAngleWidth,                                         0.0f,
                                     0.0f, 2.0f / tanAngleHeight,   (tanUp + tanDown) / tanAngleHeight,                                         0.0f,
                                     0.0f,                  0.0f,   -(farZ + offsetZ) / (farZ - nearZ), -(farZ * (nearZ + offsetZ)) / (farZ - nearZ),
                                     0.0f,                  0.0f,                                -1.0f,                                         0.0f);
    // clang-format on
  }
}

Matrix4x4 createRotation(const XrQuaternionf& quaternion) {
  const float x2 = quaternion.x + quaternion.x;
  const float y2 = quaternion.y + quaternion.y;
  const float z2 = quaternion.z + quaternion.z;

  const float xx2 = quaternion.x * x2;
  const float yy2 = quaternion.y * y2;
  const float zz2 = quaternion.z * z2;

  const float yz2 = quaternion.y * z2;
  const float wx2 = quaternion.w * x2;
  const float xy2 = quaternion.x * y2;
  const float wz2 = quaternion.w * z2;
  const float xz2 = quaternion.x * z2;
  const float wy2 = quaternion.w * y2;

  // clang-format off
  return Matrix4x4(1.0f - yy2 - zz2,        xy2 - wz2,        xz2 + wy2, 0.0f,
                          xy2 + wz2, 1.0f - xx2 - zz2,        yz2 - wx2, 0.0f,
                          xz2 - wy2,        yz2 + wx2, 1.0f - xx2 - yy2, 0.0f,
                               0.0f,             0.0f,             0.0f, 1.0f);
  // clang-format on
}

Matrix4x4 createView(const XrPosef& pose) {
  const auto rotation = createRotation(pose.orientation);
  const auto translation =
      Matrix4x4::translation(pose.position.x, pose.position.y, pose.position.z);
  auto tr = translation * rotation;

  Matrix4x4 out;
  out(0, 0) = tr(0, 0);
  out(1, 0) = tr(0, 1);
  out(2, 0) = tr(0, 2);
  out(3, 0) = 0.0f;

  out(0, 1) = tr(1, 0);
  out(1, 1) = tr(1, 1);
  out(2, 1) = tr(1, 2);
  out(3, 1) = 0.0f;

  out(0, 2) = tr(2, 0);
  out(1, 2) = tr(2, 1);
  out(2, 2) = tr(2, 2);
  out(3, 2) = 0.0f;

  out(0, 3) =
      -(tr(0, 0) * tr(0, 3) + tr(1, 0) * tr(1, 3) + tr(2, 0) * tr(2, 3));
  out(1, 3) =
      -(tr(0, 1) * tr(0, 3) + tr(1, 1) * tr(1, 3) + tr(2, 1) * tr(2, 3));
  out(2, 3) =
      -(tr(0, 2) * tr(0, 3) + tr(1, 2) * tr(1, 3) + tr(2, 2) * tr(2, 3));
  out(3, 3) = 1.0f;
  return out;
}

TINYXR_NAMESPACE_CLOSE
