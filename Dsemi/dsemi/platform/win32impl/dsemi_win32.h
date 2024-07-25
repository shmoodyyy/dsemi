#ifndef DSEMI_PLATFORM_WINDOWS_INCLUDE_H
#define DSEMI_PLATFORM_WINDOWS_INCLUDE_H
#include "dsemi/core/dsemiwindows.h"
#include <unordered_map>

namespace dsemi::win32
{
    template<typename T>
    class Handles
    {
    public:
        void add(void* addr, T val)
        {
            if (auto found = m_map.find(addr); found == m_map.end())
                m_map.emplace(addr, val);
        }

        auto get(void* addr) -> T
        {
            if (auto found = m_map.find(addr); found != m_map.end())
                return found->second;
            return {};
        }

        auto remove(void* addr)
        {
            if (auto found = m_map.find(addr); found != m_map.end())
                m_map.erase(found);
        }

    private:
        static std::unordered_map<std::uintptr_t, T> m_map;
    };

    static Handles<HWND> windowHandles;
}

#endif
