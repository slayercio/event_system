#pragma once
#include <evs/evs.hpp>
#include <evs/event.hpp>
#include <functional>

namespace evs
{
    template<typename DataType = evs::Event<>::data_type, typename IdType = evs::Event<>::id_type>
    struct EventListener
    {
    public:
        using handler_type = std::function<bool(const DataType& data)>;

    public:
        EventListener(handler_type handler, IdType id);

        bool Execute(const evs::Event<DataType, IdType>& event) const;

        IdType EventId() const;

    private:
        IdType       m_EventId;
        handler_type m_Handler;
    };
}

namespace evs
{
    template<typename DataType, typename IdType>
    EventListener<DataType, IdType>::EventListener(std::function<bool(const DataType& data)> handler, IdType id)
    {
        EVS_DEBUG_CALL();
        m_Handler = handler;
        m_EventId = id;
    }

    template<typename DataType, typename IdType>
    bool EventListener<DataType, IdType>::Execute(const evs::Event<DataType, IdType>& event) const
    {
        EVS_DEBUG_CALL();
        auto data = event.Data();

        return m_Handler(data);
    }

    template<typename DataType, typename IdType>
    IdType EventListener<DataType, IdType>::EventId() const
    {
        EVS_DEBUG_CALL();
        return m_EventId;
    }
}
