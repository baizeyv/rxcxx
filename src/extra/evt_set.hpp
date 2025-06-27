//
// Created by baizeyv on 6/27/2025.
//

#ifndef EVT_SET_HPP
#define EVT_SET_HPP
#include <unordered_map>

#include "rx_event.hpp"

namespace rxcxx::extra{
    template<class TKey, class TValue>
    class evt_set {
        std::unordered_map<TKey, rx_event<TValue> > map;

    public:
        evt_set() = default;

        ~evt_set() = default;

        void operator()(const TKey& key, const TValue& v) const {
            if (const auto it = map.find(key); it != map.end()) {
                it->second(v);
            }
        }

        void operator()(const TKey& key, TValue&& v) const {
            if (const auto it = map.find(key); it != map.end()) {
                it->second(std::move(v));
            }
        }

        void operator()(TKey&& key, const TValue& v) const {
            if (const auto it = map.find(std::move(key)); it != map.end()) {
                it->second(v);
            }
        }

        void operator()(TKey&& key, TValue&& v) const {
            if (const auto it = map.find(std::move(key)); it != map.end()) {
                it->second = std::move(v);
            }
        }

        rx_event<TValue>& operator[](const TKey index) {
            if (const auto it = map.find(index); it == map.end()) {
                map[index] = rx_event<TValue>();
            }
            return map[index];
        }
    };
}

#endif //EVT_SET_HPP
