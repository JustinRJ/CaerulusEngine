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
        Camera::Camera() :
            m_view(mat4()),
            m_proj(mat4())
        {
            SetFOV(CAMERA_INIT_FOV);
            SetAspect(CAMERA_INIT_ASPECT);
            SetNear(CAMERA_INIT_NEAR);
            SetFar(CAMERA_INIT_FAR);

            m_view = lookAt(
                CAMERA_INIT_POSITION,
                CAMERA_INIT_FORWARD + CAMERA_INIT_POSITION,
                CAMERA_INIT_UP);

            m_proj = perspective(
                radians(CAMERA_INIT_FOV),
                CAMERA_INIT_ASPECT,
                CAMERA_INIT_NEAR,
                CAMERA_INIT_FAR);

            SetAperture(CAMERA_INIT_APETURE);
            SetShutterSpeed(CAMERA_INIT_SHUTTER_SPEED);
            SetISO(CAMERA_INIT_ISO);
        }

        void Camera::Translate(const vec3& translation, bool translateY)
        {
            mat4& view = m_view.GetMatrix();
            vec3 tempPos(view[X][W], view[Y][W], view[Z][W]);
            tempPos += normalize(vec3(view[X][X], translateY ? view[X][Z] : 0.0f, view[Z][X])) * translation.x;
            if (translateY)
            {
                tempPos += normalize(vec3(view[X][Y], view[Y][Y], view[Z][Y])) * translation.y;
            }
            tempPos += normalize(vec3(view[X][Z], translateY ? view[Y][Z] : 0.0f, view[Z][Z])) * translation.z;
            view *= translate(mat4(1.0f), -tempPos);
        }

        void Camera::Rotate(const vec3& eulerDelta, const vec3& forcedUp)
        {
            Transform model = inverse(m_view.GetMatrix());
            quat orig_rot = normalize(quat_cast(model.GetMatrix()));

            bool force = l1Norm(forcedUp) > 0.0f;

            quat yaw = angleAxis(radians(-eulerDelta.x), force ? forcedUp : UpVector(orig_rot));
            quat pitch = angleAxis(radians(-eulerDelta.y), RightVector(orig_rot));
            quat roll = angleAxis(radians(eulerDelta.z), ForwardVector(orig_rot));
            quat rotation = yaw * pitch * roll * orig_rot;

            if (force)
            {
                float dotVal = dot(UpVector(normalize(rotation)), forcedUp) - CAMERA_CORRECTION;
                if (dotVal < 0.0f)
                {
                    float adjustment = (acosf(dotVal) - half_pi<float>())
                        * (dot(forcedUp, ForwardVector(rotation)) < 0.0f ? 1.0f : -1.0f);

                    rotation = angleAxis(adjustment, RightVector(rotation)) * rotation;
                }
            }

            Transform temp = mat4_cast(rotation);

            temp.SetAxis(model.GetAxis(Index::W), Index::W);

            m_view = inverse(temp.GetMatrix());
        }

        const mat4& Camera::GetViewMatrix() const
        {
            return m_view.GetMatrix();
        }

        const mat4& Camera::GetProjMatrix() const
        {
            return m_proj;
        }

        vec3 Camera::GetPosition() const
        {
            return m_view.GetTranslation();
        }

        vec3 Camera::GetForward() const
        {
            return m_view.GetColumn(Index::Z);
        }

        vec3 Camera::GetUp() const
        {
            return m_view.GetColumn(Index::Y);
        }

        float Camera::GetFOV() const
        {
            return m_degFOV;
        }

        float Camera::GetAspect() const
        {
            return m_aspect;
        }

        float Camera::GetNear() const
        {
            return m_near;
        }

        float Camera::GetFar() const
        {
            return m_far;
        }

        void Camera::SetPosition(const vec3& position)
        {
            m_view.SetTranslation(position);
        }

        void Camera::SetForward(const vec3& forward)
        {
            m_view.SetColumn(forward, Index::Z);
        }

        void Camera::SetUp(const vec3& up)
        {
            m_view.SetColumn(up, Index::Y);
        }

        void Camera::SetFOV(float fov)
        {
            m_degFOV = fov;
            m_proj = perspective(m_degFOV, m_aspect, m_near, m_far);
        }

        void Camera::SetAspect(float aspect)
        {
            m_aspect = aspect;
            m_proj = perspective(m_degFOV, m_aspect, m_near, m_far);
        }

        void Camera::SetNear(float nearP)
        {
            m_near = nearP;
            m_proj = perspective(m_degFOV, m_aspect, m_near, m_far);
        }

        void Camera::SetFar(float farP)
        {
            m_far = farP;
            m_proj = perspective(m_degFOV, m_aspect, m_near, m_far);
        }

        float Camera::GetAperture() const
        {
            return m_aperture;
        }

        float Camera::GetShutterSpeed() const
        {
            return m_shutterSpeed;
        }

        float Camera::GetISO() const
        {
            return m_ISO;
        }

        void Camera::SetAperture(float aperture)
        {
            m_aperture = aperture;
        }

        void Camera::SetShutterSpeed(float shutterSpeed)
        {
            m_shutterSpeed = shutterSpeed;
        }

        void Camera::SetISO(float iso)
        {
            m_ISO = iso;
        }
    }
}