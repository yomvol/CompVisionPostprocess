#pragma once
#include <functional>
#include <vector>

namespace utils
{
    template <typename... Args>
    class Event {
    public:
        using EventHandler = std::function<void(Args...)>;

        inline void operator()(Args... params) const { trigger(params...); }
        inline void operator+=(const EventHandler& callback) { subscribe(callback); }

        void subscribe(const EventHandler& callback) {
            callbacks.push_back(callback);
        }

        void trigger(Args... args) const {
            for (const auto& callback : callbacks) {
                callback(args...);
            }
        }

    private:
        std::vector<EventHandler> callbacks;
    };
}