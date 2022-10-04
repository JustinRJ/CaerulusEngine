#pragma once

#define CAERULUS_NETWORKING __declspec(dllexport)

#include <string>
#include <vector>
#include <cstring>

namespace Networking
{
    class INetworkable
    {
    public:
        virtual ~INetworkable() = default;

        virtual void serialize(char** dataOut, uint32_t& size) const = 0;
        virtual void deserialize(char** dataIn) = 0;

        template <class T>
        static void appendData(char** dataOut, uint32_t& size, const T& data)
        {
            static_assert(std::is_fundamental<T>::value, "appendData only supports primitives!");
            appendData(dataOut, size, &data, sizeof(T));
        }

        template <class T>
        static void extractData(char** dataIn, T& data)
        {
            static_assert(std::is_fundamental<T>::value, "extractData only supports primitives!");
            extractData(dataIn, &data, sizeof(T));
        }

        template <class T>
        static void appendVector(char** dataOut, uint32_t& size, const std::vector<T>& vec)
        {
            static_assert(std::is_fundamental<T>::value, "appendVector only supports primitives!");
            uint32_t vSize = vec.size();
            appendData<uint32_t>(dataOut, size, vSize);
            if (vSize != 0)
            {
                appendData(dataOut, size, vec.data(), vSize);
            }
        }

        template <class T>
        static void extractVector(char** dataIn, std::vector<T>& vec)
        {
            static_assert(std::is_fundamental<T>::value, "extractVector only supports primitives!");
            uint32_t size = 0;
            extractData<uint32_t>(dataIn, size);
            if (size != 0)
            {
                vec.resize(size);
                extractData(dataIn, vec.data(), size);
            }
        }

        static void appendString(char** dataOut, uint32_t& size, const std::string& str)
        {
            uint32_t sSize = str.size();
            appendData<uint32_t>(dataOut, size, sSize);
            if (sSize != 0)
            {
                appendData(dataOut, size, str.data(), sSize);
            }
        }

        static void extractString(char** dataIn, std::string& str)
        {
            uint32_t size = 0;
            extractData<uint32_t>(dataIn, size);
            if (size != 0)
            {
                str.resize(size);
                extractData(dataIn, str.data(), size);
            }
        }

        static void appendStringVector(char** dataOut, uint32_t& size, const std::vector<std::string>& vec)
        {
            appendData<uint32_t>(dataOut, size, vec.size());
            for (const auto& str : vec)
            {
                appendString(dataOut, size, str);
            }
        }

        static void extractStringVector(char** dataIn, std::vector<std::string>& vec)
        {
            uint32_t size = 0;
            extractData<uint32_t>(dataIn, size);
            if (size != 0)
            {
                vec.resize(size);
                for (auto& str : vec)
                {
                    extractString(dataIn, str);
                }
            }
        }

    private:
        static void appendData(char** dataOut, uint32_t& size, const void* data, size_t sizeT)
        {
            std::memcpy(*dataOut, data, sizeT);
            *dataOut += sizeT;
            size += sizeT;
        }

        static void extractData(char** dataIn, void* data, size_t sizeT)
        {
            std::memcpy(data, *dataIn, sizeT);
            *dataIn += sizeT;
        }
    };
}