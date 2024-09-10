#pragma once

#include <evs/debug.hpp>
#include <functional>
#include <memory>
#include <vector>
namespace evs
{
    template<typename DataType>
    class CSEvent
    {
    public:
        using handler_type = std::function<bool(std::shared_ptr<DataType>)>;

        void operator+=(handler_type handler)
        {
            On(handler);
        }

        void On(handler_type handler)
        {
            EVS_DEBUG_CALL();

            m_Handlers.push_back(handler);
        }

        void Invoke(const DataType&& data)
        {
            EVS_DEBUG_CALL();

            for(const auto& handler : m_Handlers)
            {
                handler(std::make_shared<DataType>(data));
            }
        }

    private:
        std::vector<handler_type> m_Handlers;
    };
}
