//
// Created by baizeyv on 6/27/2025.
//

#ifndef MSG_SET_HPP
#define MSG_SET_HPP
#include <unordered_map>

#include "rx_message.hpp"

namespace rxcxx::extra {
    template<class T>
    class msg_set {
        std::unordered_map<T, rx_message> map;

    public:
        msg_set() = default;

        ~msg_set() = default;

        void operator()(const T &key) const {
            if (const auto it = map.find(key); it != map.end()) {
                it->second();
            }
        }

        void operator()(T &&key) const {
            if (const auto it = map.find(std::move(key)); it != map.end()) {
                it->second();
            }
        }

        rx_message &operator[](const T index) {
            if (const auto it = map.find(index); it == map.end()) {
                map[index] = rx_message();
            }
            return map[index];
        }
    };
}

#endif //MSG_SET_HPP
