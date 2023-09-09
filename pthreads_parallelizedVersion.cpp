#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int N = 1000;
#define NUM_THREADS 4

struct MultiplTask {
    vector<vector<int>> &A;  // vector of vectors = 2D
    vector<vector<int>> &B;
    vector<vector<int>> &C;
    int start;
    int end;
};

// Matrix Multiply Thread Method
void *matrixMultiply(void *arg) {
    MultiplTask *task = static_cast<MultiplTask*>(arg);
    
    // Perform matrix multiplication for the assigned range
    for (int i = task->start; i < task->end; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                task->C[i][j] += task->A[i][k] * task->B[k][j];
            }
        }
    }
    
    pthread_exit(NULL);
}

int main() {
    // Create matrices A, B, and C
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    // Random Number Generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 9);

    // Initialize matrices A and B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    // Thread Management
    pthread_t threads[NUM_THREADS];
    int partition_size = N / NUM_THREADS;

    auto start = high_resolution_clock::now();

    // Create and start matrix multiplication threads
    for (int i = 0; i < NUM_THREADS; i++) {
        MultiplTask task{A, B, C, i * partition_size, (i + 1) == NUM_THREADS ? N : (i + 1) * partition_size};
        pthread_create(&threads[i], NULL, matrixMultiply, &task);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    // File Output
    ofstream outputFile("results.txt", ios::binary);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << " " << C[i][j] << " ";
        }
        outputFile << endl;
    }
    
    outputFile.close();

    // Print execution time
    cout << "Parallel execution time: " << duration.count() << " ms" << endl;

    return 0;
}
