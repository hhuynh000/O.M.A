#pragma once

#include <vector>
#include <span>

namespace core
{
    template<typename T>
    class UidMap
    {
    public:
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        
        // Iterators
        const_iterator begin() const { return m_elements.begin(); }
        const_iterator end() const { return m_elements.end(); }

        // Element access
        std::span<const T> elements() const { return std::span<const T>(m_elements); }; 
        std::span<const int> uids() const { return std::span<int>(m_uids); }; 

        // Capacity 
        bool empty() const { return m_uids.empty(); }
        size_t size() const { return m_uids.size(); }

        // Modifiers
        void insert(int uid, const T& element);
        void insert(int uid, T&& element);
        void erase(int uid);
        void clear();

        // Lookup
        iterator at(int uid);
        const_iterator at(int uid) const;
        bool contains(int uid) const;

    private:
        std::vector<T> m_elements;
        std::vector<int> m_uids;
    };

    template<typename T>
    void UidMap<T>::insert(const int uid, const T& element)
    {
        iterator lower_bound = std::lower_bound(m_uids.begin(), m_uids.end(), uid);
        if (lower_bound != m_uids.end())
        {
            return;
        }

        iterator insert_position =
            std::next(m_elements.begin(), std::distance(m_uids.begin(), lower_bound));

        m_uids.insert(lower_bound, uid);
        m_elements.insert(insert_position, element);
    }

    template<typename T>
    void UidMap<T>::insert(const int uid, T&& element)
    {
        iterator lower_bound = std::lower_bound(m_uids.begin(), m_uids.end(), uid);
        if (lower_bound != m_uids.end())
        {
            return;
        }

        iterator insert_element_position =
            std::next(m_elements.begin(), std::distance(m_uids.begin(), lower_bound));

        m_uids.insert(lower_bound, uid);
        m_elements.insert(insert_element_position, element);
    }

    template<typename T>
    void UidMap<T>::erase(int uid)
    {
        iterator lower_bound = std::lower_bound(m_uids.begin(), m_uids.end());
        if (lower_bound == m_uids.end())
        {
            return;
        }

        iterator erase_element_position = 
            std::next(m_elements.begin(), std::distance(m_uids.begin(), lower_bound));
        
        m_uids.erase(lower_bound);
        m_elements.erase(erase_element_position);
    }

    template<typename T>
    void UidMap<T>::clear()
    {
        m_elements.clear();
        m_uids.clear();
    }

    template<typename T>
    typename UidMap<T>::iterator UidMap<T>::at(const int uid)
    {
        const_iterator lower_bound = std::lower_bound(m_uids.cbegin(), m_uids.cend(), uid);
        if (lower_bound == m_uids.cend())
        {
            return m_elements.end();
        }

        return std::next(m_elements.begin(), std::distance(m_uids.cbegin(), lower_bound));
    }

    template<typename T>
    typename UidMap<T>::const_iterator UidMap<T>::at(const int uid) const
    {
        const_iterator lower_bound = std::lower_bound(m_uids.cbegin(), m_uids.cend(), uid);
        if (lower_bound == m_uids.cend())
        {
            return m_elements.cend();
        }

        return std::next(m_elements.cbegin(), std::distance(m_uids.cbegin(), lower_bound));
    }

    template<typename T>
    bool UidMap<T>::contains(const int uid) const
    {
        const_iterator lower_bound = std::lower_bound(m_uids.cbegin(), m_uids.cend(), uid);
        if (lower_bound == m_uids.cend())
        {
            return false;
        }

        return *lower_bound == uid;
    }
}
