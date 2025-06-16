// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <immintrin.h> 
#define ASCENDING 1
#define DESCENDING 0

void compareAndSwap_SIMD(int* arr, int offset, int dist, int dir) {

    __m256i a = _mm256_loadu_si256((__m256i*)(arr + offset));

    __m256i b = _mm256_loadu_si256((__m256i*)(arr + offset + dist));

    __m256i min_val = _mm256_min_epi32(a, b);
    __m256i max_val = _mm256_max_epi32(a, b);
    if (dir == ASCENDING) {
        _mm256_storeu_si256((__m256i*)(arr + offset), min_val);
        _mm256_storeu_si256((__m256i*)(arr + offset + dist), max_val);
    }
    else {
        _mm256_storeu_si256((__m256i*)(arr + offset), max_val);
        _mm256_storeu_si256((__m256i*)(arr + offset + dist), min_val);
    }
}
void bitonicMerge_SIMD(int* arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;


        for (int i = low; i < low + k; i += 8) {

            compareAndSwap_SIMD(arr, i, k, dir);
        }

        bitonicMerge_SIMD(arr, low, k, dir);
        bitonicMerge_SIMD(arr, low + k, k, dir);
    }
}

void bitonicSort_SIMD(int* arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        bitonicSort_SIMD(arr, low, k, ASCENDING);
        bitonicSort_SIMD(arr, low + k, k, DESCENDING);
        bitonicMerge_SIMD(arr, low, cnt, dir);
    }
}

int main() {
    const int N = 24;
    int* data = (int*)_mm_malloc(sizeof(int) * N, 32);

    srand(time(0));
    for (int i = 0; i < N; i++) {
        data[i] = rand() % 1000;
    }

    std::cout << "Original data:\n";
    for (int i = 0; i < N; i++) std::cout << data[i] << " ";

    std::cout << "\n";
    double start = omp_get_wtime();
    bitonicSort_SIMD(data, 0, N, ASCENDING);
    double end = omp_get_wtime();
    int mid = N / 2;

    std::cout << "Sorted data: ";
    for (int i = 0; i < N; i++) std::cout << data[i] << " ";
    std::cout << "\nTime: " << end - start << "s\n";



    std::cout << std::endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
