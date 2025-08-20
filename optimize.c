#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Pdh.h>
#include <PdhMsg.h>

#pragma comment(lib,"pdh.lib")

#define ITERATIONS 200000000
#define INTERVAL 100

double GetCpuUsage(int CoreIndex, HQUERY query, HCOUNTER counter) {
    PDH_FMT_COUNTERVALUE value;
    PdhCollectQueryData(query);

    PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);

    return value.doubleValue;
}

void FindBestCore() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    int cores = info.dwNumberOfProcessors;
    printf("[+] Number of Logical Processors : %d\n", cores);

    HQUERY* queries = (HQUERY*)malloc(cores * sizeof(HQUERY));
    HCOUNTER* counters = (HCOUNTER*)malloc(cores * sizeof(HCOUNTER));

    for (int i = 0; i < cores; i++) {
        char Path[128];
        sprintf(Path, "\\Processor(%d)\\%% Processor Time", i);

        PdhOpenQueryA(NULL, 0, &queries[i]);
        PdhAddCounterA(queries[i], Path, 0, &counters[i]);
    }

    // Delay For Initilization

    Sleep(1000);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    printf("[+] Thread Priority Set to Highest [+]\n");

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    volatile double sum = 0;

    for (long long i = 0; i < ITERATIONS; i++) {
        // Periodically check CPU load and move to idle core
        if (i % 10000000 == 0) { // Every 10M iterations
            int idleCore = 0;
            double minLoad = 100.0;

            for (int j = 0; j < cores; j++) {
                double load = GetCpuUsage(j, queries[j], counters[j]);

                if (load < minLoad) {
                    minLoad = load;
                    idleCore = j;
                }
            }

            DWORD_PTR mask = 1ULL << idleCore;
            SetThreadAffinityMask(GetCurrentThread(), mask);

            printf("Iteration %lld: moved to core %d (load %.2f%%)\n", i, idleCore, minLoad);
            Sleep(INTERVAL);
        }

        sum += i * 0.000001;
    }
    
    QueryPerformanceCounter(&end);
    double total = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart;

    printf("Computation done, sum = %f\n", sum);
    printf("[+] Execution Time is %.3f ...\n", total);

    for (int i = 0; i < cores; i++)
        PdhCloseQuery(queries[i]);

    free(queries);
    free(counters);
}

int main(void) {
    FindBestCore();

    return 0;
}
