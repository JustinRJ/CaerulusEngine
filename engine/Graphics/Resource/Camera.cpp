#include "stdafx.h"

#include "Camera.h"

namespace
{
    using namespace Core::Math;
    const vec3 CAMERA_INIT_POSITION = vec3(0.0f, 0.0f, 0.0f);
    const vec3 CAMERA_INIT_FORWARD = vec3(0.0f, 0.0f, -1.0f);
    const vec3 CAMERA_INIT_UP = vec3(0.0f, 1.0f, 0.0f);

    const float CAMERA_INIT_ASPECT = (16.0f / 9.0f);
    const float CAMERA_INIT_FOV = 54.0f;
    const float CAMERA_INIT_NEAR = 1.0f;    //THIS IS SET IN SHADER G-BUFFER.FRAG
    const float CAMERA_INIT_FAR = 1000.0f;  //THIS IS SET IN SHADER G-BUFFER.FRAG

    float CAMERA_INIT_APETURE = 16.0f;
    float CAMERA_INIT_SHUTTER_SPEED = 0.5f;
    float CAMERA_INIT_ISO = 1000.0f;

    const float CAMERA_CORRECTION = 0.1f;
}

namespace Graphics
{
    namespace Resource
    {
        using namespace Core::Math;

        Camera::Camera() :
            m_View(mat4()),
            m_Proj(mat4())
        {
            SetFOV(CAMERA_INIT_FOV);
            SetAspect(CAMERA_INIT_ASPECT);
            SetNear(CAMERA_INIT_NEAR);
            SetFar(CAMERA_INIT_FAR);

            SetViewMatrix(lookAt(
                CAMERA_INIT_POSITION,
                CAMERA_INIT_FORWARD + CAMERA_INIT_POSITION,
                CAMERA_INIT_UP));

            SetProjMatrix(perspective(
                radians(CAMERA_INIT_FOV),
                CAMERA_INIT_ASPECT,
                CAMERA_INIT_NEAR,
                CAMERA_INIT_FAR));

            SetAperture(CAMERA_INIT_APETURE);
            SetShutterSpeed(CAMERA_INIT_SHUTTER_SPEED);
            SetISO(CAMERA_INIT_ISO);
        }

        Camera::~Camera()
        {
        }

        void Camera::TranslateXZ(const vec3& translation)
        {
            vec3 tempPos(m_View[X][W], m_View[Y][W], m_View[Z][W]);
            tempPos += normalize(vec3(m_View[X][X], 0.0f, m_View[Z][X])) * translation.x;
            tempPos += normalize(vec3(m_View[X][Z], 0.0f, m_View[Z][Z])) * translation.z;
            m_View *= translate(mat4(1.0f), -tempPos);
        }

        void Camera::Translate(const vec3& translation)
        {
            vec3 tempPos(m_View[X][W], m_View[Y][W], m_View[Z][W]);
            tempPos = tempPos + normalize(vec3(m_View[X][X], m_View[X][Z], m_View[Z][X])) * translation.x;
            tempPos = tempPos + normalize(vec3(m_View[X][Y], m_View[Y][Y], m_View[Z][Y])) * translation.y;
            tempPos = tempPos + normalize(vec3(m_View[X][Z], m_View[Y][Z], m_View[Z][Z])) * translation.z;
            m_View *= translate(mat4(1.0f), -tempPos);
        }

        void Camera::Rotate(const vec3& eulerDelta, const vec3& forcedUp)
        {
            mat4 model = inverse(m_View);
            quat orig_rot = normalize(quat_cast(model));

            bool force = l1Norm(forcedUp) > 0.0f;

            quat yaw = angleAxis(radians(-eulerDelta.x), force ? forcedUp : MathHelper::UpVector(orig_rot));
            quat pitch = angleAxis(radians(-eulerDelta.y), MathHelper::RightVector(orig_rot));
            quat roll = angleAxis(radians(eulerDelta.z), MathHelper::ForwardVector(orig_rot));
            quat rotation = yaw * pitch * roll * orig_rot;

            if (force)
            {
                float dotVal = dot(MathHelper::UpVector(normalize(rotation)), forcedUp) - CAMERA_CORRECTION;
                if (dotVal < 0.0f)
                {
                    float adjustment = (acosf(dotVal) - half_pi<float>())
                        * (dot(forcedUp, MathHelper::ForwardVector(rotation)) < 0.0f ? 1.0f : -1.0f);

                    rotation = angleAxis(adjustment, MathHelper::RightVector(rotation)) * rotation;
                }
            }

            mat4 temp = mat4_cast(rotation);

            MathHelper::SetAxis(temp, MathHelper::GetAxis(model, Index::W), Index::W);

            m_View = inverse(temp);
        }

        void Camera::GetProjViewModel(mat4 & out) const
        {
            mat4 cameraTransform;
            MathHelper::CreateTansform(cameraTransform, GetPosition(), GetForward(), vec3(1.0f));
            out = m_Proj * m_View * cameraTransform;
        }

        const mat4& Camera::GetViewMatrix() const
        {
            return m_View;
        }

        const mat4& Camera::GetProjMatrix() const
        {
            return m_Proj;
        }

        vec3 Camera::GetPosition() const
        {
            return MathHelper::GetTranslation(m_View);
        }

        void Camera::SetViewMatrix(const mat4& view)
        {
            m_View = view;
        }
        void Camera::SetProjMatrix(const mat4& proj)
        {
            m_Proj = proj;
        }

        vec3 Camera::GetForward() const
        {
            return MathHelper::GetColumn(m_View, Index::Z);
        }

        vec3 Camera::GetUp() const
        {
            return MathHelper::GetColumn(m_View, Index::Y);
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

        void Camera::SetPosition(const vec3& position)
        {
            MathHelper::SetTranslation(m_View, position);
        }

        void Camera::SetForward(const vec3& forward)
        {
            MathHelper::SetColumn(m_View, forward, Index::Z);
        }

        void Camera::SetUp(const vec3& up)
        {
            MathHelper::SetColumn(m_View, up, Index::Y);
        }

        void Camera::SetFOV(float fov)
        {
            m_DegFOV = fov;
            m_Proj = perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetAspect(float aspect)
        {
            m_Aspect = aspect;
            m_Proj = perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetNear(float nearP)
        {
            m_Near = nearP;
            m_Proj = perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
        }

        void Camera::SetFar(float farP)
        {
            m_Far = farP;
            m_Proj = perspective(m_DegFOV, m_Aspect, m_Near, m_Far);
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