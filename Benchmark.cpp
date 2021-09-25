/* This is a small benchmark to see the performance of FArray. If you want to generate a CSV-file (to create a diagram), enable CREATE_CSV_DEBUG by setting it to true.
 * If you just want to see the timings for one iteration without generate a CSV-file, set CREATE_CSV_DEBUG to false.
 */
#define CREATE_CSV_DEBUG false

#if CREATE_CSV_DEBUG
    #include <fstream>
#endif

void FArrayBenchmark()
{
#if CREATE_CSV_DEBUG
    std::ofstream CSVChart("FArray_Timings.csv");
#endif

#if CREATE_CSV_DEBUG
    for (int i = 1; i <= 10000; i++)
    {
#endif
#if CREATE_CSV_DEBUG
        const int ArraySize = i;
#else
        // Here you can set the target Array-Size. This size is the default size of the Array and also the insertion-num.
        const int ArraySize = 10000;
#endif
#if CREATE_CSV_DEBUG
        long long Timing_FArray = 0;
        long long Timing_Vector = 0;
#endif
        std::vector<int> t;

        ff::FArray<int> FastArray(ArraySize);

        for (int i = 0; i < ArraySize; i++)
        {
            FastArray[i] = rand();
            t.push_back(rand());
        }

        std::cout << std::endl;

#if CREATE_CSV_DEBUG
        std::cout << i << std::endl;
#endif

        {
            std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < ArraySize; i++)
            {
                FastArray.Insert(i, 10);
            }

            FastArray.Optimization();

            for (size_t i = 0; i < FastArray.Size(); i++)
            {
                FastArray[i] += 1;
            }

            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            auto ElapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
            std::cout << "Time Elapsed: " << ElapsedTime << "[ns]";
            std::cout << "(" << (double(ElapsedTime) / 1000.0 / 1000.0) << "[ms])" << std::endl;
#if CREATE_CSV_DEBUG
            Timing_FArray = ElapsedTime;
#endif
        }

        {
            std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < ArraySize; i++)
            {
                t.insert(t.begin() + i, 10);
            }

            for (size_t i = 0; i < t.size(); i++)
            {
                t[i] += 1;
            }

            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            auto ElapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
            std::cout << "Time Elapsed: " << ElapsedTime << "[ns]";
            std::cout << "(" << (double(ElapsedTime) / 1000.0 / 1000.0) << "[ms])" << std::endl;
#if CREATE_CSV_DEBUG
            Timing_Vector = ElapsedTime;
#endif
        }

#if CREATE_CSV_DEBUG
        CSVChart << ArraySize << "," << Timing_FArray << "," << Timing_Vector << std::endl;
#endif

#if CREATE_CSV_DEBUG
    }
#endif

#if CREATE_CSV_DEBUG
    CSVChart.close();
#endif
}

int main()
{
    FArrayBenchmark();
    return 0;
}
