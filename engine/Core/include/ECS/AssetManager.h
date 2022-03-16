#pragma once

#include "ECS/Manager.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class AssetManager : public Manager<const std::string, R>
        {
        public:
            AssetManager() = default;
            virtual ~AssetManager() = default;

            void Insert(const std::string key, std::unique_ptr<R>&& value) override
            {
                if (!key.empty() && value)
                {
                    Manager<const std::string, R>::Insert(key, std::move(value));
                }
            }
        };
    }
}