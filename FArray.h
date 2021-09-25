// Copyright by fischfroschi

#pragma once

#include "FArrayHelpers.h"
#include "FAlgorithm.h"

namespace ff
{
    template <typename T>
    class FArray
    {
    public:
        FArray();
        FArray(size_t s);
        ~FArray();
        FArrayIteraor<T> begin();
        FArrayIteraor<T> end();
        void Insert(size_t l, T val);
        void Remove(size_t l);
        void Add(T val);
        void Add_Front(T val);
        T& operator[](int i);
        size_t Size();
        T* GetArray(bool bReuse = false);
        void Clean(bool bReorg = false);
        void Close();
        void Reopen(size_t s);
        void Optimization();
        void Sort();
        void Remove_Val(T val);
        void Remove_If(std::function<bool(T x)> Condition);
    private:
        T* InitArray = nullptr;
        size_t ArraySize;
        size_t OrigArraySize;
        FArrayBlock<T> ArrayBlock;
        FArrayElemBlock<T> ArrayElemBlock;
        FArrayElem<T>* FirstElem;
        FArrayElem<T>* LastElem;
    };


    /*
    * ---------------------------------------------------------------------------------------------------------
    * Implementation of FArray
    * ---------------------------------------------------------------------------------------------------------
    */

    template<typename T>
    inline ff::FArray<T>::FArray()
    {
    }

    template <typename T>
    FArray<T>::FArray(size_t s)
    {
        if (s > 0)
        {
            FirstElem = ArrayElemBlock.GetNext(ArrayBlock.GetStartBlock(s), s, nullptr);
            LastElem = FirstElem;
            ArraySize = s;
            OrigArraySize = s;

            InitArray = FirstElem->Memadress();
        }
    }

    template <typename T>
    FArray<T>::~FArray()
    {
        Clean();
    }

    template <typename T>
    FArrayIteraor<T> FArray<T>::begin()
    {
        return FArrayIteraor<T>(FirstElem);
    }

    template <typename T>
    FArrayIteraor<T> FArray<T>::end()
    {
        return FArrayIteraor<T>(LastElem->next());
    }

    template <typename T>
    void FArray<T>::Insert(size_t l, T val)
    {
        if (l < 0 || l > ArraySize) return;

        if (l == 0)
        {
            if (!FirstElem->Insert(0, val))
            {
                FirstElem = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, FirstElem);
            }

            ArraySize++;
            return;
        }
        else if (l == ArraySize)
        {
            if (!LastElem->Insert(LastElem->Size(), val))
            {
                LastElem->next() = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, nullptr);
                LastElem = LastElem->next();
            }

            ArraySize++;
            return;
        }

        /* Check if Array is "complicated" */
        if ((ArraySize - OrigArraySize) >= 20480) Optimization();

        size_t StoredSize = 0;

        for (FArrayElem<T>* e : *this)
        {
            l -= StoredSize;
            StoredSize = e->Size();

            if (l <= FARRAYELEMBLOCK || l <= StoredSize)
            {
                if (l == StoredSize)
                {
                    if (!e->Insert(StoredSize, val))
                    {
                        FArrayElem<T>* temp = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, e->next());
                        e->next() = temp;
                    }

                    ArraySize++;
                    return;
                }

                if (!e->Insert(l, val))
                {
                    /* Shift already allocated memory (temp); Insert 9 at pos 4 (new value is temp2):
                     * |(Size = 3) 10 40 5| |(Size = 5) 6 9 1 10 29|
                     * =
                     * |(Size = 3) 10 40 5| |([new] Size = 1) 6| |(Size = 1) 9| |(Size = 4) 9 1 10 29|
                     */

                    FArrayElem<T>* temp = ArrayElemBlock.GetNext(e->Memadress() + l, StoredSize - l, e->next(), false);
                    FArrayElem<T>* temp2 = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, temp);

                    if (!e->next())
                    {
                        LastElem = temp;
                    }

                    e->next() = temp2;
                    e->Resize(l);
                }

                ArraySize++;
                return;
            }
        }
    }

    template <typename T>
    void FArray<T>::Remove(size_t l)
    {
        /* Elements will be freed after Calling Optimaziation or Clean */

        if (l < 0 || l >= ArraySize) return;

        if (l == 0)
        {
            if (FirstElem->Size() <= 1)
            {
                FirstElem = FirstElem->next();
            }
            else
            {
                FirstElem->Memadress()++;
                FirstElem->Resize(FirstElem->Size() - 1);
            }

            ArraySize--;
            return;
        }
        else if (l == (ArraySize - 1))
        {
            if (LastElem->Size() <= 1)
            {
                FArrayElem<T>* temp = nullptr;

                for (FArrayElem<T>* e : *this)
                {
                    if (!e->next())
                    {
                        temp = e;
                        break;
                    }
                }

                LastElem = temp;
            }
            else
            {
                LastElem->Resize(LastElem->Size() - 1);
            }

            ArraySize--;
            return;
        }

        /* Not important if -1; handled before */
        size_t StoredSize = 0;
        FArrayElem<T>* le = nullptr;

        for (FArrayElem<T>* e : *this)
        {
            l -= StoredSize;
            StoredSize = e->Size();

            if (l < StoredSize)
            {
                if (l == 0)
                {
                    if (StoredSize <= 1)
                    {
                        FArrayElem<T>* temp = e->next();
                        le->next() = temp;
                    }
                    else
                    {
                        e->Memadress()++;
                        e->Resize(StoredSize - 1);
                    }

                    ArraySize--;
                    return;
                }
                else if (l == StoredSize - 1)
                {
                    e->Resize(StoredSize - 1);

                    ArraySize--;
                    return;
                }

                FArrayElem<T>* temp = ArrayElemBlock.GetNext(e->Memadress() + l + 1, StoredSize - l, e->next(), false);

                if (!e->next())
                {
                    LastElem = temp;
                }

                e->next() = temp;
                e->Resize(l);

                ArraySize--;
                return;
            }

            le = e;
        }
    }

    template <typename T>
    void FArray<T>::Add(T val)
    {
        LastElem->next() = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, nullptr);
        LastElem = LastElem->next();
    }

    template <typename T>
    void FArray<T>::Add_Front(T val)
    {
        FirstElem = ArrayElemBlock.GetNext(ArrayBlock.GetNext(val), 1, FirstElem);
    }

    template <typename T>
    T& FArray<T>::operator[](int i)
    {
        if (i < 0 || i >= ArraySize)
        {
            static T iv = T();
            return iv;
        }

        size_t StoredSize = 0;

        for (auto e : *this)
        {
            i -= StoredSize;
            if (i < e->Size())
            {
                return (*e)[i];
            }
            else
            {
                StoredSize = e->Size();
            }
        }
    }

    template <typename T>
    size_t FArray<T>::Size()
    {
        return ArraySize;
    }

    template <typename T>
    T* FArray<T>::GetArray(bool bReuse)
    {
        int cp = 0;

        int* out = nullptr;
        if (bReuse)
        {
            out = ArrayBlock.GetStartBlock(Size());
        }
        else
        {
            out = new T[Size()];
        }

        for (FArrayElem<T>* e : *this)
        {
            std::memcpy(out + cp, e->Memadress(), e->Size() * sizeof(T));
            cp += e->Size();
        }

        return out;
    }

    template <typename T>
    void FArray<T>::Clean(bool bReorg)
    {
        if (InitArray)
        {
            delete[] InitArray;
        }

        for (T* e : ArrayBlock.Blocks)
        {
            delete[] e;
        }

        ArrayBlock.Blocks.SmartReset();

        if (bReorg)
        {
            ArrayBlock.ReOrg();
        }

        for (FArrayElem<T>* e : ArrayElemBlock.Blocks)
        {
            delete[] e;
        }

        ArrayElemBlock.Blocks.SmartReset();

        if (bReorg)
        {
            ArrayElemBlock.ReOrg();
        }

        ArraySize = 0;
    }

    template <typename T>
    void FArray<T>::Close()
    {
        Clean();
    }

    template <typename T>
    void FArray<T>::Reopen(size_t s)
    {
        ArrayBlock.ReOrg();
        ArrayElemBlock.ReOrg();

        FirstElem = ArrayElemBlock.GetNext(ArrayBlock.GetStartBlock(s), s, nullptr);
        LastElem = FirstElem;
        ArraySize = s;
        OrigArraySize = s;

        InitArray = FirstElem->Memadress();
    }

    template <typename T>
    void FArray<T>::Optimization()
    {
        const size_t Cache = ArraySize;
        T* p = GetArray(true);
        Clean(true);
        FirstElem = ArrayElemBlock.GetNext(p, Cache, nullptr);
        LastElem = FirstElem;
        ArraySize = Cache;
        OrigArraySize = Cache;

        InitArray = FirstElem->Memadress();
    }

    template<typename T>
    inline void FArray<T>::Sort()
    {
        Optimization();
        ff::UltraQuickSort<T>(FirstElem->Memadress(), Size());
    }

    template<typename T>
    inline void FArray<T>::Remove_Val(T val)
    {
        int i = 0;
        while (i < Size())
        {
            if ((*this)[i] == val)
            {
                Remove(i);
            }
            else
            {
                i++;
            }
        }
    }

    template<typename T>
    inline void FArray<T>::Remove_If(std::function<bool(T x)> Condition)
    {
        int i = 0;
        while (i < Size())
        {
            if (Condition((*this)[i]))
            {
                Remove(i);
            }
            else
            {
                i++;
            }
        }
    }
}