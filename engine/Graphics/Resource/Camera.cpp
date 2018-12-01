#include "stdafx.h"

#include "Camera.h"

const float CAMERA_CORRECTION = 0.1f;

namespace Graphics
{
    namespace Resource
    {
        using namespace Core::Math;

        Camera::Camera() :
            m_View(*new glm::mat4()),
            m_Proj(*new glm::mat4())
        {
        }

        Camera::~Camera()
        {
        }

        void Camera::TranslateXZ(const glm::vec3& translation)
        {
            glm::vec3 tempPos(m_View[X][W], m_View[Y][W], m_View[Z][W]);
            tempPos += normalize(glm::vec3(m_View[X][X], 0.0f, m_View[Z][X])) * translation.x;
            tempPos += normalize(glm::vec3(m_View[X][Z], 0.0f, m_View[Z][Z])) * translation.z;
            m_View *= translate(glm::mat4(1.0f), -tempPos);
        }

        void Camera::Translate(const glm::vec3& translation)
        {
            glm::vec3 tempPos(m_View[X][W], m_View[Y][W], m_View[Z][W]);
            tempPos = tempPos + normalize(glm::vec3(m_View[X][X], m_View[X][Z], m_View[Z][X])) * translation.x;
            tempPos = tempPos + normalize(glm::vec3(m_View[X][Y], m_View[Y][Y], m_View[Z][Y])) * translation.y;
            tempPos = tempPos + normalize(glm::vec3(m_View[X][Z], m_View[Y][Z], m_View[Z][Z])) * translation.z;
            m_View *= translate(glm::mat4(1.0f), -tempPos);
        }

        void Camera::Rotate(const glm::vec3& eulerDelta, const glm::vec3& forcedUp)
        {
            glm::mat4 model = glm::inverse(m_View);
            glm::quat orig_rot = glm::normalize(glm::quat_cast(model));

            bool force = glm::l1Norm(forcedUp) > 0.0f;

            glm::quat yaw = glm::angleAxis(glm::radians(-eulerDelta.x), force ? forcedUp : MathHelper::UpVector(orig_rot));
            glm::quat pitch = glm::angleAxis(glm::radians(-eulerDelta.y), MathHelper::RightVector(orig_rot));
            glm::quat roll = glm::angleAxis(glm::radians(eulerDelta.z), MathHelper::ForwardVector(orig_rot));
            glm::quat rotation = yaw * pitch * roll * orig_rot;

            if (force)
            {
                float dot = glm::dot(MathHelper::UpVector(glm::normalize(rotation)), forcedUp) - CAMERA_CORRECTION;
                if (dot < 0.0f)
                {
                    float adjustment = (acosf(dot) - glm::half_pi<float>())
                        * (glm::dot(forcedUp, MathHelper::ForwardVector(rotation)) < 0.0f ? 1.0f : -1.0f);

                    rotation = glm::angleAxis(adjustment, MathHelper::RightVector(rotation)) * rotation;
                }
            }

            glm::mat4 temp = glm::mat4_cast(rotation);

            MathHelper::SetAxis(temp, MathHelper::GetAxis(model, Index::W), Index::W);

            m_View = glm::inverse(temp);
        }

        const glm::mat4& Camera::GetViewMatrix() const
        {
            return m_View;
        }

        const glm::mat4& Camera::GetProjMatrix() const
        {
            return m_Proj;
        }

        glm::vec3 Camera::GetPosition() const
        {
            return glm::vec3(MathHelper::GetTranslation(m_View));
        }

        void Camera::SetViewMatrix(const glm::mat4& view)
        {
            m_View = view;
        }
        void Camera::SetProjMatrix(const glm::mat4& proj)
        {
            m_Proj = proj;
        }

        glm::vec3 Camera::GetForward() const
        {
            return glm::vec3(MathHelper::GetColumn(m_View, Index::Z));
        }

        glm::vec3 Camera::GetUp() const
        {
            return glm::vec3(MathHelper::GetColumn(m_View, Index::Y));
        }

        float Camera::GetFOV() const
        {
            return m_DegFOV;
        }

        float Camera::GetAspect() const
        {
            return m_Aspect;
        }

        float Camera::GetNear() const
        {
            return m_Near;
        }

        float Camera::GetFar() const
        {
            return m_Far;
        }

        void Camera::SetPosition(const glm::vec3& position)
        {
            MathHelper::SetTranslation(m_View, position);
        }

        void Camera::SetForward(const glm::vec3& forward)
        {
            MathHelper::SetColumn(m_View, forward, Index::Z);
        }

        void Camera::SetUp(const glm::vec3& up)
        {
            MathHelper::SetColumn(m_View, up, Index::Y);
        }

        void Camera::SetFOV(float fov)
        {
            m_DegFOV = fov;
            m_Proj = glm::perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetAspect(float aspect)
        {
            m_Aspect = aspect;
            m_Proj = glm::perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetNear(float nearP)
        {
            m_Near = nearP;
            m_Proj = glm::perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetFar(float farP)
        {
            m_Far = farP;
            m_Proj = glm::perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        float Camera::GetAperture() const
        {
            return m_Aperture;
        }

        float Camera::GetShutterSpeed() const
        {
            return m_ShutterSpeed;
        }

        float Camera::GetISO() const
        {
            return m_ISO;
        }

        void Camera::SetAperture(float aperture)
        {
            m_Aperture = aperture;
        }

        void Camera::SetShutterSpeed(float shutterSpeed)
        {
            m_ShutterSpeed = shutterSpeed;
        }

        void Camera::SetISO(float iso)
        {
            m_ISO = iso;
        }
    }
}