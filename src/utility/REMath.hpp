#pragma once

#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>

//#include "ReClass.hpp"

namespace utility::math {
using namespace glm;

static vec3 euler_angles(const glm::mat4& rot);
static float fix_angle(float ang);
static void fix_angles(const glm::vec3& angles);
static float clamp_pitch(float ang);

// RE engine's way of storing euler angles or I'm just an idiot.
static vec3 euler_angles(const glm::mat4& rot) {
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
    glm::extractEulerAngleYZX(rot, yaw, roll, pitch);

    return { pitch, yaw, roll };
}

static float fix_angle(float ang) {
    auto angDeg = glm::degrees(ang);

    while (angDeg > 180.0f) {
        angDeg -= 360.0f;
    }

    while (angDeg < -180.0f) {
        angDeg += 360.0f;
    }

    return glm::radians(angDeg);
}

static void fix_angles(glm::vec3& angles) {
    angles[0] = fix_angle(angles[0]);
    angles[1] = fix_angle(angles[1]);
    angles[2] = fix_angle(angles[2]);
}

float clamp_pitch(float ang) {
    return std::clamp(ang, glm::radians(-89.0f), glm::radians(89.0f));
}

static glm::mat4 remove_y_component(const glm::mat4& mat) {
    // Remove y component and normalize so we have the facing direction
    const auto forward_dir = glm::normalize(Vector3f{ mat[2].x, 0.0f, mat[2].z });

    return glm::rowMajor4(glm::lookAtLH(Vector3f{}, Vector3f{ forward_dir }, Vector3f(0.0f, 1.0f, 0.0f)));
}

static glm::mat4 remove_roll(const glm::mat4& mat) {
    // Extract the forward direction from the matrix
    const auto forward = glm::normalize(glm::vec3(mat[2]));

    // Fixed world up vector
    const glm::vec3 world_up(0.0f, 1.0f, 0.0f);

    // Calculate right vector with corrected cross product order
    const auto right = glm::normalize(glm::cross(world_up, forward));

    // Re-calculate the up vector to ensure orthogonality
    const auto up = glm::normalize(glm::cross(forward, right));

    // Build rotation matrix from orthonormal basis
    glm::mat4 result(1.0f);
    result[0] = glm::vec4(right, 0.0f);
    result[1] = glm::vec4(up, 0.0f);
    result[2] = glm::vec4(forward, 0.0f);
    return result;
}

static glm::mat4 remove_roll_pitch(const glm::mat4& mat) {
        // Extract the forward direction from the matrix
    const auto forward = glm::normalize(glm::vec3(mat[2].x, 0.0f, mat[2].z));

    // Fixed world up vector
    const glm::vec3 world_up(0.0f, 1.0f, 0.0f);

    // Calculate right vector with corrected cross product order
    const auto right = glm::normalize(glm::cross(world_up, forward));

    // Re-calculate the up vector to ensure orthogonality
    const auto up = glm::normalize(glm::cross(forward, right));

    // Build rotation matrix from orthonormal basis
    glm::mat4 result(1.0f);
    result[0] = glm::vec4(right, 0.0f);
    result[1] = glm::vec4(up, 0.0f);
    result[2] = glm::vec4(forward, 0.0f);
    return result;
}


static glm::mat4 extract_y_component(const glm::mat4& mat) {
    const auto forward_dir = glm::normalize(Vector3f{ mat[2].x, 0.0f, mat[2].z });

    return glm::lookAtLH(Vector3f{}, Vector3f{ forward_dir }, Vector3f(0.0f, 1.0f, 0.0f));
}

static float get_yaw(const glm::mat4& mat) {
    const auto forward_dir = glm::normalize(Vector3f{ mat[2].x, 0.0f, mat[2].z });
    return glm::atan(forward_dir.x, forward_dir.z);
}

static const  glm::mat4 permutation_pre = {
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, -1, 0, 0,
        0, 0,  0, 1
};
static const glm::mat4 permutation_post = glm::transpose(permutation_pre);

static glm::mat4 to_havok_space(const glm::mat4& mat) {
    return permutation_pre * mat * permutation_post;
}

static glm::mat4 from_havok_space(const glm::mat4& mat) {
    return permutation_post * mat * permutation_pre;
}

static quat to_quat(const vec3& v) {
    const auto mat = glm::rowMajor4(glm::lookAtLH(Vector3f{0.0f, 0.0f, 0.0f}, v, Vector3f{0.0f, 1.0f, 0.0f}));

    return glm::quat{mat};
}

static quat flatten(const quat& q) {
    const auto forward = glm::normalize(glm::quat{q} * Vector3f{ 0.0f, 0.0f, 1.0f });
    const auto flattened_forward = glm::normalize(Vector3f{forward.x, 0.0f, forward.z});
    return utility::math::to_quat(flattened_forward);
}
}