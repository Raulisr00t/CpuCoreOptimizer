#include <Windows.h>
#include <stdio.h>

#define ITERATIONS 1000000000

typedef struct {
    int x;
    char pad1[60]; 
    int y;
    char pad2[60];
} Shared;

Shared somedata = { 0, 0 };

DWORD WINAPI thread1(void* argv) {
    for (int i = 0; i < ITERATIONS; i++)
        somedata.x++;
    return 0;
}

DWORD WINAPI thread2(void* argv) {
    for (int i = 0; i < ITERATIONS; i++)
        somedata.y++;
    return 0;
}

int main() {
    HANDLE hThread1, hThread2;
    LARGE_INTEGER start, end, freq;

    QueryPerformanceFrequency(&freq);

    QueryPerformanceCounter(&start);

    hThread1 = CreateThread(NULL, 0, thread1, NULL, 0, NULL);
    hThread2 = CreateThread(NULL, 0, thread2, NULL, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    QueryPerformanceCounter(&end);

    double time = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart;

    printf("[+] DONE: Value of x: %d Value of y: %d\n", somedata.x, somedata.y);
    printf("[+] Execution time: %.6f seconds\n", time);

    return 0;
}
