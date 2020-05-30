//#pragma once
//
//#include "Transform.h"
//
//namespace Core
//{
//    namespace Math
//    {
//        class CAERULUS_CORE Cube
//        {
//        public:
//            Cube() = delete;
//            ~Cube() = default;
//
//            Cube(const mat4& transform) :
//                m_T(transform)
//            {}
//
//            Cube(const Cube& cube) :
//                m_T(cube.m_T)
//            {}
//
//            Cube operator=(const Cube& cube)
//            {
//                m_T = cube.m_T;
//                return *this;
//            }
//
//            const vec3& Min() const
//            {
//                return m_T.TransformVector(m_min);
//            }
//
//            const vec3& Max() const
//            {
//                return m_T.TransformVector(m_max);
//            }
//
//            const mat4& GetTransform() const
//            {
//                return m_T.GetMatrix();
//            }
//
//            mat4& GetTransform()
//            {
//                return m_T.GetMatrix();
//            }
//
//        private:
//            const vec3 m_min = { -0.5, -0.5, -0.5 };
//            const vec3 m_max = { 0.5, 0.5, 0.5 };
//            Transform m_T;
//        };
//    }
//}