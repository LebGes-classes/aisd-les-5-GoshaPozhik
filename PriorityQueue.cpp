#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <stdexcept>

using namespace std;

template<typename Priority, typename Data>
class PriorityQueue {
private:
    struct Item {
        Priority priority;
        Data data;
        
        Item(const Priority& p, const Data& d) : priority(p), data(d) {}
    };
    
    vector<Item> heap;
    
    size_t parent(size_t i) { return (i - 1) / 2; }
    size_t leftChild(size_t i) { return 2 * i + 1; }
    size_t rightChild(size_t i) { return 2 * i + 2; }
    
    void siftUp(size_t i) {
        while (i > 0 && heap[parent(i)].priority < heap[i].priority) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }
    
    void siftDown(size_t i) {
        size_t maxIndex = i;
        size_t left = leftChild(i);
        
        if (left < heap.size() && heap[left].priority > heap[maxIndex].priority) {
            maxIndex = left;
        }
        
        size_t right = rightChild(i);
        if (right < heap.size() && heap[right].priority > heap[maxIndex].priority) {
            maxIndex = right;
        }
        
        if (i != maxIndex) {
            swap(heap[i], heap[maxIndex]);
            siftDown(maxIndex);
        }
    }
    
public:
    PriorityQueue() {}

    void Enqueue(const Priority& priority, const Data& data) {
        heap.push_back(Item(priority, data));
        siftUp(heap.size() - 1);
    }
    
    Data Dequeue() {
        if (heap.empty()) {
            throw runtime_error("Priority queue is empty");
        }
        
        Data result = heap[0].data;
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!heap.empty()) {
            siftDown(0);
        }
        
        return result;
    }
    
    const Data& Peek() const {
        if (heap.empty()) {
            throw runtime_error("Priority queue is empty");
        }
        return heap[0].data;
    }
    
    bool isEmpty() const {
        return heap.empty();
    }
    
    size_t size() const {
        return heap.size();
    }
    
    void clear() {
        heap.clear();
    }
};

void testPerformance() {
    const int Nstart = 1000;
    const int Nend = 10000;
    const int stepN = 1000;
    const int q = 7; 

    cout << "==== Тестирование производительности Enqueue ====\n";
    cout << "N\tВремя (сек)\n";
    
    for (int N = Nstart; N <= Nend; N += stepN) {
        double* times = new double[q];

        for (int rep = 0; rep < q; rep++) {
            PriorityQueue<int, int> pq;
            
            clock_t start = clock();
            
            for (int i = 0; i < N; i++) {
                pq.Enqueue(rand() % N, i);
            }
            
            clock_t end = clock();
            times[rep] = (double)(end - start) / CLOCKS_PER_SEC;
        }

        for (int i = 0; i < q - 1; i++) {
            for (int j = 0; j < q - i - 1; j++) {
                if (times[j] > times[j + 1]) {
                    double temp = times[j];
                    times[j] = times[j + 1];
                    times[j + 1] = temp;
                }
            }
        }

        int keep = q - q / 5;
        double avg = 0;
        for (int i = 0; i < keep; i++) {
            avg += times[i];
        }
        avg /= keep;

        cout << N << "\t" << avg << " сек\n";
        
        delete[] times;
    }
    
    cout << "\n==== Тестирование производительности Dequeue ====\n";
    cout << "N\tВремя (сек)\n";
    
    for (int N = Nstart; N <= Nend; N += stepN) {
        double* times = new double[q];

        for (int rep = 0; rep < q; rep++) {
            PriorityQueue<int, int> pq;
            
            for (int i = 0; i < N; i++) {
                pq.Enqueue(rand() % N, i);
            }
            
            clock_t start = clock();
            
            while (!pq.isEmpty()) {
                pq.Dequeue();
            }
            
            clock_t end = clock();
            times[rep] = (double)(end - start) / CLOCKS_PER_SEC;
        }

        for (int i = 0; i < q - 1; i++) {
            for (int j = 0; j < q - i - 1; j++) {
                if (times[j] > times[j + 1]) {
                    double temp = times[j];
                    times[j] = times[j + 1];
                    times[j + 1] = temp;
                }
            }
        }

        int keep = q - q / 5;
        double avg = 0;
        for (int i = 0; i < keep; i++) {
            avg += times[i];
        }
        avg /= keep;

        cout << N << "\t" << avg << " сек\n";
        
        delete[] times;
    }
}

int main() {
    testPerformance();
    return 0;
}