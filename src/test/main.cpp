#include <evs/csevent.hpp>
#include <evs/event_bus.hpp>
#include <iostream>
#include <Windows.h>
#include <memory>

bool handle_int(std::shared_ptr<int> ptr)
{
    return false;
}

int main()
{
    evs::EventBus<> eventBus(true);

    eventBus.On(0, [&](std::any data) {
        std::cout << data.type().name() << std::endl;
        std::cout << std::any_cast<int>(data) << std::endl;

        return false;
    });

    eventBus.On(1, [&](std::any data) {
        std::cout << std::any_cast<double>(data) << std::endl;
        std::cout << data.type().name() << std::endl;

        return false;
    });

    evs::Event<> event(0, std::make_any<int>(10));
    evs::Event<> event2(1, std::make_any<double>(1.23));
    eventBus.Emit(event);
    std::cout << "emitted event" << std::endl;
    eventBus.Emit(event);

    evs::CSEvent<int> OnInt;
    OnInt.On([&](auto ptr) {
        EVS_DEBUG_CALL();
        std::cout << *ptr << std::endl;

        return false;
    });

    OnInt.Invoke(20);

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        if (GetAsyncKeyState(VK_HOME) & 0x8000)
        {
            eventBus.Emit(event);
            eventBus.Emit(event2);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
