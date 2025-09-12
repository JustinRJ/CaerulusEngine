#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Transform
        {
        public:
            Transform() = default;

            Transform(const vec3& scale, const quat& rotation, const vec3& translation) :
                m_isDirty(true),
                m_scale(scale),
                m_rotation(rotation),
                m_translation(translation)
            {}

            const mat4& GetMatrix() const
            {
                if (m_isDirty)
                {
                    m_isDirty = false;
                    m_matrix = translate(mat4(1.f), m_translation);
                    m_matrix = m_matrix * toMat4(m_rotation);
                    m_matrix = m_matrix * glm::scale(mat4(1.f), m_scale);
                }
                return m_matrix;
            }

            const vec3& GetScale() const
            {
                return m_scale;
            }

            void SetScale(const vec3& scale)
            {
                m_isDirty = true;
                m_scale = scale;
            }

            void Scale(const vec3& scale)
            {
                m_isDirty = true;
                m_scale *= scale;
            }

            const quat& GetRotation() const
            {
                return m_rotation;
            }

            void SetRotation(const quat& rotation)
            {
                m_isDirty = true;
                m_rotation = rotation;
            }

            void Rotate(const quat& rotation)
            {
                m_isDirty = true;
                m_rotation = glm::normalize(m_rotation * rotation);
            }

            const vec3& GetTranslation() const
            {
                return m_translation;
            }

            void SetTranslation(const vec3& translation)
            {
                m_isDirty = true;
                m_translation = translation;
            }

            void Translate(const vec3& translation)
            {
                m_isDirty = true;
                m_translation += translation;
            }

            static Transform Compose(const Transform& parent, const Transform& child)
            {
                Transform result;
                // Scale: component-wise multiplication
                result.SetScale(parent.GetScale() * child.GetScale());
                // Rotation: parent rotation applied before child rotation
                result.SetRotation(glm::normalize(parent.GetRotation() * child.GetRotation()));
                // Translation: parent translation + parent rotation applied to scaled child translation
                result.SetTranslation(parent.GetTranslation() + parent.GetRotation() * (parent.GetScale() * child.GetTranslation()));
                return result;
            }

            static Transform InverseCompose(const Transform& parent, const Transform& world)
            {
                Transform local;
                // Scale: divide component-wise
                glm::vec3 parentScale = parent.GetScale();
                glm::vec3 worldScale = world.GetScale();
                local.SetScale(worldScale / parentScale);
                // Rotation: remove parent rotation
                local.SetRotation(glm::normalize(glm::inverse(parent.GetRotation()) * world.GetRotation()));
                // Translation: remove parent translation and rotation, account for scale
                local.SetTranslation(glm::inverse(parent.GetRotation()) * (world.GetTranslation() - parent.GetTranslation()) / parentScale);
                return local;
            }

        private:

            mutable bool m_isDirty = false;
            vec3 m_scale = vec3(1.0);
            quat m_rotation = quat();
            vec3 m_translation = vec3(0.0);
            mutable mat4 m_matrix = mat4(1.f);
        };

        inline Transform operator*(const Transform& lhs, const Transform& rhs)
        {
            return Transform::Compose(lhs, rhs);
        }
    }
}
