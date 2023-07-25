#ifndef TINYXR_IMPL_OPENXR_MATH_H
#define TINYXR_IMPL_OPENXR_MATH_H

#include "tinyxr/core/matrix4x4.h"
#include "tinyxr/core/tinyxr.h"
#include "tinyxr/impl/openxr.h"

TINYXR_NAMESPACE_OPEN

XrPosef XrPosefIdentity();

Matrix4x4 createProjection(const XrFovf& fov, const float nearZ,
                           const float farZ);

Matrix4x4 createRotation(const XrQuaternionf& quaternion);

Matrix4x4 createView(const XrPosef& pose);

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_OPENXR_MATH_H
