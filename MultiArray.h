#pragma once
#include<vector>
#include <stdexcept>

namespace CMath {
    template<typename T>
    class MultiArray
    {
    public:
        template<typename... Args>
        MultiArray(const Args&... args)
        {
            m_Size = { static_cast<size_t>(args)... };
            size_t newsize = 0;
            if (!m_Size.empty() && m_Size[0] > 0)
            {
                newsize = 1;
            }
            for (auto Long : m_Size)
            {
                newsize *= Long;
            }
            m_Data.resize(newsize);
        }


        // 复制构造函数
        MultiArray(const MultiArray& other) = default;

        // 赋值运算符
        MultiArray& operator=(const MultiArray& other) = default;

        // 移动构造函数
        MultiArray(MultiArray&& other) = default;

        // 移动赋值运算符
        MultiArray& operator=(MultiArray&& other) = default;

        // 调整数组的大小
        template<typename... Args>
        void resizeValue(const T& value, const Args&... args) {
            m_Size = { static_cast<size_t>(args)... };
            size_t newsize = 0;
            if (!m_Size.empty() && m_Size[0] > 0)
            {
                newsize = 1;
            }
            for (auto Long : m_Size)
            {
                newsize *= Long;
            }
            m_Data.resize(newsize, value);
        }
        // 调整数组的大小
        template<typename... Args>
        void resize(const Args&... args) {
            m_Size = { static_cast<size_t>(args)... };
            size_t newsize = 0;
            if (!m_Size.empty() && m_Size[0] > 0)
            {
                newsize = 1;
            }
            for (auto Long : m_Size)
            {
                newsize *= Long;
            }
            
            m_Data.resize(newsize);
        }
        //返回指定维度的长度
        size_t size(size_t level)const{
            if (level >= m_Size.size())
            {
                throw "索引越界";
            }
            return m_Size[level];
        }
        // 返回数组的长度或大小
        size_t size() const {
            return m_Data.size();
        }

        // 检查数组是否为空
        bool empty() const {
            return m_Data.empty();
        }
        // 清空数组中的所有元素
        void clear() {
            m_Data.clear();
            m_Size.clear();
        }
        // 返回数组的第一个元素
        T& front() {
            return m_Data.front();
        }

        // 返回数组的最后一个元素
        T& back() {
            return m_Data.back();
        }

        // 返回数组的第一个元素（const 版本）
        const T& front() const {
            return m_Data.front();
        }

        // 返回数组的最后一个元素（const 版本）
        const T& back() const {
            return m_Data.back();
        }

        typename std::vector<T>::iterator begin() {
            return m_Data.begin();
        }

        typename std::vector<T>::iterator end() {
            return m_Data.end();
        }

        typename std::vector<T>::const_iterator begin() const {
            return m_Data.begin();
        }

        typename std::vector<T>::const_iterator end() const {
            return m_Data.end();
        }

        template<typename Index, typename... Indices>
        T& At(Index index, const Indices&... indices)
        {
            static_assert(sizeof...(Indices) + 1 == std::tuple_size_v<std::array<size_t, sizeof...(Indices) + 1>>, "索引不正确");
            return AtImpl(index, indices...);
        }
        template<typename Index, typename... Indices>
        const T& At(Index index, const Indices&... indices)const
        {
            static_assert(sizeof...(Indices) + 1 == std::tuple_size_v<std::array<size_t, sizeof...(Indices) + 1>>, "索引不正确");
            return AtImpl(index, indices...);
        }
        template<typename... Indices>
        T& operator[](const Indices&... indices) {
            return At(indices...);
        }

        template<typename... Indices>
        const T& operator[](const Indices&... indices) const {
            return At(indices...);
        }
    private:
        template<typename Index>
        T& AtImpl(Index index)
        {
            if (index >= m_Data.size())
            {
                throw std::out_of_range("索引越界");
            }
            return m_Data[index];
        }

        template<typename Index, typename... Indices>
        T& AtImpl(Index index, const Indices&... indices)
        {
            if (sizeof...(Indices) >= m_Size.size())
            {
                throw std::out_of_range("索引缺失");
            }

            if (index >= m_Size[sizeof...(Indices)])
            {
                throw std::out_of_range("索引越界");
            }

            size_t flatIndex = index;
            size_t stride = 1;
            size_t i = 0;
            ((flatIndex += indices * (stride *= m_Size[i++])), ...);
            if (flatIndex >= m_Data.size())
            {
                throw std::out_of_range("索引越界");
            }
            return m_Data[flatIndex];
        }
        template<typename Index>
        const T& AtImpl(Index index)const
        {
            if (index >= m_Size[0])
            {
                throw std::out_of_range("索引越界");
            }
            return m_Data[index];
        }

        template<typename Index, typename... Indices>
        const T& AtImpl(Index index, const Indices&... indices)const
        {
            if (sizeof...(Indices) >= m_Size.size())
            {
                throw std::out_of_range("索引缺失");
            }

            if (index >= m_Size[sizeof...(Indices)])
            {
                throw std::out_of_range("索引越界");
            }

            size_t flatIndex = index;
            size_t stride = 1;
            size_t i = 0;
            ((flatIndex += indices * (stride *= m_Size[i++])), ...);
            if (flatIndex >= m_Data.size())
            {
                throw std::out_of_range("索引越界");
            }
            return m_Data[flatIndex];
        }
    private:
        std::vector<T> m_Data;
        std::vector<size_t> m_Size;
    };


}