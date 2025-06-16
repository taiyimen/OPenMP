// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#define ASCENDING 1
#define DESCENDING 0

void compareAndSwap(std::vector<int>& arr, int i, int j, int dir) {
    if ((dir == ASCENDING && arr[i] > arr[j]) || (dir == DESCENDING && arr[i] < arr[j])) {

        std::swap(arr[i], arr[j]);
    }
}
void bitonicMerge(std::vector<int>& arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
#pragma omp parallel for
        for (int i = low; i < low + k; ++i) {
            compareAndSwap(arr, i, i + k, dir);
        }
        bitonicMerge(arr, low, k, dir);

        bitonicMerge(arr, low + k, k, dir);
    }
}

void bitonicSort(std::vector<int>& arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        bitonicSort(arr, low, k, ASCENDING);


        bitonicSort(arr, low + k, k, DESCENDING);

        bitonicMerge(arr, low, cnt, dir);
    }
}

int main() {
    const int N = 20;
    std::vector<int> data(N);
    srand(time(0));
    for (int i = 0; i < N; i++) data[i] = rand() % 1000;
    std::cout << "Original data:";
    for (int i = 0; i < N; ++i) std::cout << data[i] << " ";
    std::cout << "\n";
    double start = omp_get_wtime();
    bitonicSort(data, 0, N, ASCENDING);
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
