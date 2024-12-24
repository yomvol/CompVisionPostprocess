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

    private:
        void subscribe(const EventHandler& callback) {
            callbacks.push_back(callback);
        }

        void trigger(Args... args) const {
            for (const auto& callback : callbacks) {
                callback(args...);
            }
        }

        std::vector<EventHandler> callbacks;
    };

    // Helper function to convert std::function<void()> to std::function<void(bool)>
    static std::function<void(bool)> convert(const std::function<void()>& func) {
        return [func](bool) {
            func();
            };
    }
}