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


        // ���ƹ��캯��
        MultiArray(const MultiArray& other) = default;

        // ��ֵ�����
        MultiArray& operator=(const MultiArray& other) = default;

        // �ƶ����캯��
        MultiArray(MultiArray&& other) = default;

        // �ƶ���ֵ�����
        MultiArray& operator=(MultiArray&& other) = default;

        // ��������Ĵ�С
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
        // ��������Ĵ�С
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
        //����ָ��ά�ȵĳ���
        size_t size(size_t level)const{
            if (level >= m_Size.size())
            {
                throw "����Խ��";
            }
            return m_Size[level];
        }
        // ��������ĳ��Ȼ��С
        size_t size() const {
            return m_Data.size();
        }

        // ��������Ƿ�Ϊ��
        bool empty() const {
            return m_Data.empty();
        }
        // ��������е�����Ԫ��
        void clear() {
            m_Data.clear();
            m_Size.clear();
        }
        // ��������ĵ�һ��Ԫ��
        T& front() {
            return m_Data.front();
        }

        // ������������һ��Ԫ��
        T& back() {
            return m_Data.back();
        }

        // ��������ĵ�һ��Ԫ�أ�const �汾��
        const T& front() const {
            return m_Data.front();
        }

        // ������������һ��Ԫ�أ�const �汾��
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
            static_assert(sizeof...(Indices) + 1 == std::tuple_size_v<std::array<size_t, sizeof...(Indices) + 1>>, "��������ȷ");
            return AtImpl(index, indices...);
        }
        template<typename Index, typename... Indices>
        const T& At(Index index, const Indices&... indices)const
        {
            static_assert(sizeof...(Indices) + 1 == std::tuple_size_v<std::array<size_t, sizeof...(Indices) + 1>>, "��������ȷ");
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
                throw std::out_of_range("����Խ��");
            }
            return m_Data[index];
        }

        template<typename Index, typename... Indices>
        T& AtImpl(Index index, const Indices&... indices)
        {
            if (sizeof...(Indices) >= m_Size.size())
            {
                throw std::out_of_range("����ȱʧ");
            }

            if (index >= m_Size[sizeof...(Indices)])
            {
                throw std::out_of_range("����Խ��");
            }

            size_t flatIndex = index;
            size_t stride = 1;
            size_t i = 0;
            ((flatIndex += indices * (stride *= m_Size[i++])), ...);
            if (flatIndex >= m_Data.size())
            {
                throw std::out_of_range("����Խ��");
            }
            return m_Data[flatIndex];
        }
        template<typename Index>
        const T& AtImpl(Index index)const
        {
            if (index >= m_Size[0])
            {
                throw std::out_of_range("����Խ��");
            }
            return m_Data[index];
        }

        template<typename Index, typename... Indices>
        const T& AtImpl(Index index, const Indices&... indices)const
        {
            if (sizeof...(Indices) >= m_Size.size())
            {
                throw std::out_of_range("����ȱʧ");
            }

            if (index >= m_Size[sizeof...(Indices)])
            {
                throw std::out_of_range("����Խ��");
            }

            size_t flatIndex = index;
            size_t stride = 1;
            size_t i = 0;
            ((flatIndex += indices * (stride *= m_Size[i++])), ...);
            if (flatIndex >= m_Data.size())
            {
                throw std::out_of_range("����Խ��");
            }
            return m_Data[flatIndex];
        }
    private:
        std::vector<T> m_Data;
        std::vector<size_t> m_Size;
    };


}