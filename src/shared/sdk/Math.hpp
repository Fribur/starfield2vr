#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/rotate_vector.hpp>

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;
using Matrix3x3f = glm::mat3x3;
using Matrix3x4f = glm::mat3x4;
using Matrix4x4f = glm::mat4x4;


static auto mat_cast_to_havok_coord(const glm::quat& rot) {
    return glm::quat{ rot.w, -rot.x, rot.z, -rot.y };
}

static auto mat_cast_to_havok_coord(const glm::mat4& mat) {
    auto qua    = glm::normalize(glm::quat_cast(mat));
    qua = mat_cast_to_havok_coord(qua);
    glm::mat4 result = glm::mat4_cast(qua);
    result[3][0] = mat[3][0];
    result[3][1] = -mat[3][2];
    result[3][2] = mat[3][1];
    return result;
}


static glm::vec3 euler_angles_from_steamvr(const glm::mat4x4& rot) {
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    glm::extractEulerAngleYXZ(rot, yaw, pitch, roll);

    return { pitch, -yaw, -roll };
}

static glm::vec3 euler_angles_from_steamvr(const glm::quat& q) {
    const auto m = glm::mat4x4{q};
    return euler_angles_from_steamvr(m);
}
