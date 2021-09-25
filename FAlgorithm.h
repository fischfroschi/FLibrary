#pragma once

namespace ff
{
    template <typename T>
    T* UltraQuickSort(T* Array, size_t ArraySize)
    {
        int LargestValue = -1;
        int SmallestValue = -1;

        for (size_t i = 0; i < ArraySize; i++)
        {
            if (Array[i] > LargestValue) LargestValue = Array[i];
            if (Array[i] < SmallestValue || SmallestValue == -1) SmallestValue = Array[i];
        }

        const size_t NSize = LargestValue - SmallestValue + 1;

        T* t1 = new T[NSize]();
        T* t2 = new T[NSize]();

        for (size_t i = 0; i < ArraySize; i++)
        {
            if (t1[Array[i] - SmallestValue] != int())
            {
                t2[Array[i] - SmallestValue]++;
            }
            else
            {
                t1[Array[i] - SmallestValue] = Array[i];
            }
        }

        int ci = 0;
        for (size_t i = 0; i < NSize; i++)
        {
            if (t1[i] != T())
            {
                do
                {
                    Array[ci++] = t1[i];
                } while (t2[i]-- > 0);

            }
        }

        delete[] t1;
        delete[] t2;

        return Array;
    }
}