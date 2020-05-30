//#pragma once
//
//#include "Transform.h"
//
//namespace Core
//{
//    namespace Math
//    {
//        class CAERULUS_CORE Rect
//        {
//        public:
//            Rect() = delete;
//            ~Rect() = default;
//
//            Rect(const mat3& transform) :
//                m_T(transform)
//            {}
//
//            Rect(const Rect& rect) :
//                m_T(rect.m_T)
//            {}
//
//            Rect operator=(const Rect& rect)
//            {
//                m_T = rect.m_T;
//                return *this;
//            }
//
//            const vec2& Min() const
//            {
//                
//            }
//
//            const vec2& Max() const
//            {
//                
//            }
//
//            const mat3& GetTransform() const
//            {
//                return m_T;
//            }
//
//            mat3& GetTransform()
//            {
//                return m_T;
//            }
//
//        private:
//            const vec2 m_min = { -0.5, -0.5 };
//            const vec2 m_max = { 0.5, 0.5 };
//            mat3 m_T;
//        };
//    }
//}