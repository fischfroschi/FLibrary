// Copyright by fischfroschi

#pragma once

#define FARRAYELEMBLOCK 8

#define FARRAYBLOCKS 256
/* This is: @ActualSize * @FARRAYELEMBLOCK */
#define FARRAYBLOCKSIZE FARRAYBLOCKS * FARRAYELEMBLOCK

#define FARRAYELEMBLOCKSIZE 255

#include <vector>

namespace ff
{
    template <typename T>
    class FArray;

    template <typename T>
    class FArrayElem
    {
    public:
        FArrayElem();
        FArrayElem(T* m, size_t s, FArrayElem* ne, bool bUseCapacity = true);
        void Resize(size_t s);
        size_t Size();
        T*& Memadress();
        T& operator[](int i);
        FArrayElem<T>*& next();
        void SetUseCapacity(bool bUseCapacity);
        bool Insert(size_t pos, T val);
    protected:
        FArrayElem* NextElem;
        T* Memloc;
        size_t ArraySize;
        bool bCapacityBase = true;
    };

    template <typename T>
    class FArrayIteraor
    {
    public:
        FArrayIteraor(FArrayElem<T>* ptr) : p(ptr) {}
        FArrayElem<T>* operator*() const { return p; }
        FArrayElem<T>* operator->() { return p; }
        FArrayIteraor<T>& operator++() { p = p->next(); return *this; }
        FArrayIteraor<T> operator++(int) { FArrayIteraor<T> tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const FArrayIteraor<T>& a, const FArrayIteraor<T>& b) { return a.p == b.p; };
        friend bool operator!= (const FArrayIteraor<T>& a, const FArrayIteraor<T>& b) { return a.p != b.p; };
    private:
        FArrayElem<T>* p;
    };

#define SMARTARRAYDEFAULTCAPACITY 4

    template <typename T>
    class SmartArray;

    template <typename T>
    class FSmartArrayIteraor
    {
    public:
        FSmartArrayIteraor(T* ptr) : p(ptr) {}
        T operator*() const { return *p; }
        T* operator->() { return p; }
        FSmartArrayIteraor<T>& operator++() { p++; return *this; }
        FSmartArrayIteraor<T> operator++(int) { FSmartArrayIteraor<T> tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const FSmartArrayIteraor<T>& a, const FSmartArrayIteraor<T>& b) { return a.p == b.p; };
        friend bool operator!= (const FSmartArrayIteraor<T>& a, const FSmartArrayIteraor<T>& b) { return a.p != b.p; };
    private:
        T* p;
    };

    template <typename T>
    class SmartArray
    {
    public:
        SmartArray();
        ~SmartArray();
        FSmartArrayIteraor<T> begin();
        FSmartArrayIteraor<T> end();
        void Add(T val);
        T& Get(size_t pos);
        T& GetLast();
        void SmartReset();
    protected:
        size_t SmartArraySize = 0;
        unsigned char CurrentCapcity = 0;
        T* SmartPointer = nullptr;
    };

    template <typename T>
    class FArrayBlock
    {
        friend FArray<T>;
    public:
        FArrayBlock();
        T* GetNext(T Val);
        T* GetStartBlock(size_t Suggestion);
    protected:
        void ReOrg();
        short ShiftedBlock;
        SmartArray<T*> Blocks;
    };

#define FARRAYELEMBLOCKSIZE 255

    template <typename T>
    class FArrayElemBlock
    {
        friend FArray<T>;
    public:
        FArrayElemBlock();
        FArrayElem<T>* GetNext(T* Mempointer, size_t s, FArrayElem<T>* ne, bool bUseCapacity = true);
    protected:
        void ReOrg();
        unsigned char ShiftedBlock;
        SmartArray<FArrayElem<T>*> Blocks;
    };


    /*
    * ---------------------------------------------------------------------------------------------------------
    * Implementation of SmartArray
    * ---------------------------------------------------------------------------------------------------------
    */

    template<typename T>
    inline SmartArray<T>::SmartArray()
    {
        SmartPointer = new T[SMARTARRAYDEFAULTCAPACITY];
        CurrentCapcity = 1;
    }

    template<typename T>
    inline SmartArray<T>::~SmartArray()
    {
        delete[] SmartPointer;
    }

    template<typename T>
    inline FSmartArrayIteraor<T> SmartArray<T>::begin()
    {
        return FSmartArrayIteraor<T>(&SmartPointer[0]);
    }

    template<typename T>
    inline FSmartArrayIteraor<T> SmartArray<T>::end()
    {
        return FSmartArrayIteraor<T>(&SmartPointer[SmartArraySize]);
    }

    template<typename T>
    inline void SmartArray<T>::Add(T val)
    {
        if (SmartArraySize >= CurrentCapcity * SMARTARRAYDEFAULTCAPACITY)
        {
            CurrentCapcity++;
            T* temp = new T[CurrentCapcity * SMARTARRAYDEFAULTCAPACITY];
            std::memcpy(temp, SmartPointer, SmartArraySize * sizeof(T));
            delete[] SmartPointer;
            SmartPointer = temp;
        }

        SmartPointer[SmartArraySize] = val;
        SmartArraySize++;
    }

    template<typename T>
    inline T& SmartArray<T>::Get(size_t pos)
    {
        return SmartPointer[pos];
    }

    template<typename T>
    inline T& SmartArray<T>::GetLast()
    {
        return SmartPointer[SmartArraySize - 1];
    }

    template<typename T>
    inline void SmartArray<T>::SmartReset()
    {
        SmartArraySize = 0;
    }


    /*
    * ---------------------------------------------------------------------------------------------------------
    * Implementation of FArrayElem
    * ---------------------------------------------------------------------------------------------------------
    */

    template <typename T>
    FArrayElem<T>::FArrayElem()
    {
        NextElem = nullptr;
        Memloc = nullptr;
        ArraySize = 0;
    }

    template <typename T>
    FArrayElem<T>::FArrayElem(T* m, size_t s, FArrayElem* ne, bool bUseCapacity)
    {
        NextElem = ne;
        Memloc = m;
        ArraySize = s;
        bCapacityBase = bUseCapacity;
    }

    template <typename T>
    void FArrayElem<T>::Resize(size_t s)
    {
        ArraySize = s;
    }

    template <typename T>
    size_t FArrayElem<T>::Size()
    {
        return ArraySize;
    }

    template <typename T>
    T*& FArrayElem<T>::Memadress()
    {
        return Memloc;
    }

    template <typename T>
    T& FArrayElem<T>::operator[](int i)
    {
        return *(Memadress() + i);
    }

    template <typename T>
    FArrayElem<T>*& FArrayElem<T>::next()
    {
        return NextElem;
    }

    template <typename T>
    void FArrayElem<T>::SetUseCapacity(bool bUseCapacity)
    {
        bCapacityBase = bUseCapacity;
    }

    template <typename T>
    bool FArrayElem<T>::Insert(size_t pos, T val)
    {
        if (pos < 0 || pos >= FARRAYELEMBLOCK || !bCapacityBase) return false;

        if (Size() < FARRAYELEMBLOCK)
        {
            if (pos >= Size())
            {
                Memadress()[pos] = val;

                ArraySize++;
                return true;
            }
            else
            {
                for (int i = (pos + 1); i <= Size(); i++)
                {
                    Memadress()[i] = Memadress()[i - 1];
                }

                Memadress()[pos] = val;

                ArraySize++;
                return true;
            }
        }

        return false;
    }


    /*
    * ---------------------------------------------------------------------------------------------------------
    * Implementation of FArrayBlock
    * ---------------------------------------------------------------------------------------------------------
    */

    template <typename T>
    FArrayBlock<T>::FArrayBlock()
    {
        ReOrg();
    }

    template <typename T>
    T* FArrayBlock<T>::GetNext(T Val)
    {
        if (ShiftedBlock >= FARRAYBLOCKSIZE)
        {
            ReOrg();
        }

        T* a = (Blocks.GetLast() + ShiftedBlock);
        ShiftedBlock += FARRAYELEMBLOCK;
        *a = Val;
        return a;
    }

    template <typename T>
    T* FArrayBlock<T>::GetStartBlock(size_t Suggestion)
    {
        if (Suggestion < FARRAYELEMBLOCK)
        {
            Suggestion = FARRAYELEMBLOCK;
        }

        return new T[Suggestion];
    }

    template <typename T>
    void FArrayBlock<T>::ReOrg()
    {
        Blocks.Add(new T[FARRAYBLOCKSIZE]);
        ShiftedBlock = 0;
    }


    /*
    * ---------------------------------------------------------------------------------------------------------
    * Implementation of FArrayElemBlock
    * ---------------------------------------------------------------------------------------------------------
    */

    template <typename T>
    FArrayElemBlock<T>::FArrayElemBlock()
    {
        ReOrg();
    }

    template <typename T>
    FArrayElem<T>* FArrayElemBlock<T>::GetNext(T* Mempointer, size_t s, FArrayElem<T>* ne, bool bUseCapacity)
    {
        if (ShiftedBlock >= FARRAYELEMBLOCKSIZE)
        {
            ReOrg();
        }

        FArrayElem<T>* a = (Blocks.GetLast() + ShiftedBlock++);
        a->Memadress() = Mempointer;
        a->Resize(s);
        a->next() = ne;
        a->SetUseCapacity(bUseCapacity);

        return a;
    }

    template <typename T>
    void FArrayElemBlock<T>::ReOrg()
    {
        Blocks.Add(new FArrayElem<T>[FARRAYELEMBLOCKSIZE]);
        ShiftedBlock = 0;
    }
}