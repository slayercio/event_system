#pragma once
#include <evs/evs.hpp>
#include <evs/event.hpp>
#include <evs/event_listener.hpp>
#include <memory>

namespace evs
{
    template<typename DataType = evs::Event<>::data_type, typename IdType = evs::Event<>::id_type>
    struct EventBus
    {
    public:
        using event_handler_type  = evs::EventListener<DataType, IdType>::handler_type;
        using event_type          = evs::Event<DataType, IdType>;
        struct EventBusImpl;

    public:
        EventBus(bool threaded);

        void On(IdType eventId, event_handler_type handler);
        void Emit(const event_type& event);

    private:
        std::shared_ptr<EventBusImpl> m_Impl;
    };
}

#include <memory>
#include <mutex>
#include <deque>
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

namespace evs
{
    template<typename DataType, typename IdType>
    struct EventBus<DataType, IdType>::EventBusImpl
    {
        using event_handler_type = evs::EventListener<DataType, IdType>::handler_type;

        virtual void Emit(const evs::Event<DataType, IdType>& event) {}
        virtual void On(IdType eventId, event_handler_type handler) {}
    };

    template<typename DataType, typename IdType>
    struct ThreadedEventBus : public EventBus<DataType, IdType>::EventBusImpl
    {
        ThreadedEventBus()
        {
            std::cout << "ThreadedEventBus()" << std::endl;
            m_Running = true;
            m_Thread = std::thread(&ThreadedEventBus<DataType, IdType>::WaitForEvents, this);

            std::unique_lock<std::mutex> lock(m_ListenersMutex);
            m_CondVar.wait(lock); // wait for the event thread to initialize
        }

        ~ThreadedEventBus()
        {
            std::cout << "~ThreadedEventBus()" << std::endl;
            m_Running = false;
            m_Thread.join();
        }

        void WaitForEvents()
        {
            std::unique_lock<std::mutex> lock(m_EventsMutex);
            std::cout << "WaitForEvents()" << std::endl;

            auto emitEvent = [&, this] {
                std::cout << "emitEvent()" << std::endl;

                if(m_Events.size() > 0)
                {
                    auto event = m_Events.front();
                    m_Events.pop_front();

                    EmitEvent(event);
                }
            };


            m_CondVar.notify_one();
            while(m_Running)
            {
                m_CondVar.wait(lock);
                emitEvent();
            }
        }

        void EmitEvent(const evs::Event<DataType, IdType>& event)
        {
            std::scoped_lock lock(m_ListenersMutex);

            auto id = event.Id();

            bool handled = false;

            for(const auto& handler : m_Listeners)
            {
                if(!handled && handler.EventId() == id)
                {
                    handled = handler.Execute(event);
                }
            }
        }

        virtual void Emit(const evs::Event<DataType, IdType>& event) override
        {
            {
                std::scoped_lock lock(m_EventsMutex);
                m_Events.push_back(event);
            }

            m_CondVar.notify_one();
        }

        virtual void On(IdType eventId, std::function<bool(const DataType& data)> handler) override
        {
            std::scoped_lock lock(m_ListenersMutex);

            m_Listeners.push_back(evs::EventListener<DataType, IdType>(handler, eventId));
        }

    private:
        std::thread m_Thread;
        std::atomic_bool m_Running;
        std::condition_variable m_CondVar;
        std::mutex m_ListenersMutex;
        std::mutex m_EventsMutex;
        std::vector<evs::EventListener<DataType, IdType>> m_Listeners;
        std::deque<evs::Event<DataType, IdType>> m_Events;
    };

    template<typename DataType, typename IdType>
    struct NonThreadedEventBus : public EventBus<DataType, IdType>::EventBusImpl
    {
        NonThreadedEventBus()
        {
        }

        virtual void Emit(const evs::Event<DataType, IdType>& event) override
        {
            auto id = event.Id();

            bool handled = false;

            for(const auto& handler : m_Listeners)
            {
                if(!handled && handler.EventId() == id)
                {
                    handled = handler.Execute(event);
                }
            }
        }

        virtual void On(IdType eventId, std::function<bool(const DataType& data)> handler) override
        {
            m_Listeners.push_back(evs::EventListener<DataType, IdType>(handler, eventId));
        }

    private:
        std::vector<evs::EventListener<DataType, IdType>> m_Listeners;
    };

    template<typename DataType, typename IdType>
    EventBus<DataType, IdType>::EventBus(bool threaded)
    {
        if(threaded)
        {
            m_Impl = std::make_shared<ThreadedEventBus<DataType, IdType>>();
        }
        else
        {
            m_Impl = std::make_shared<NonThreadedEventBus<DataType, IdType>>();
        }
    }

    template<typename DataType, typename IdType>
    void EventBus<DataType, IdType>::On(IdType eventId, event_handler_type handler)
    {
        m_Impl->On(eventId, handler);
    }

    template<typename DataType, typename IdType>
    void EventBus<DataType, IdType>::Emit(const event_type& event)
    {
        m_Impl->Emit(event);
    }
}
