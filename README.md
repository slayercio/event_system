# Event System
Using event bus
```cpp
evs::EventBus<> eventBus(true); // threaded event bus

eventBus.On(0, [&](std::any data) {
    std::cout << std::any_cast<int>(data) << std::endl;
});

evs::Event<> event(0, std::make_any<int>(10));
eventBus.Emit(event);

```
