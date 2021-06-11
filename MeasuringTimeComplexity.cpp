#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;


const int MAX_RANDOM_VALUE = 1000;
const int RANDOM_PRECISION_NUMBERS = 2;


template <typename TFunc>
nanoseconds time_of_execution(TFunc sort) {
    auto start = high_resolution_clock::now();
    sort();
    auto stop = high_resolution_clock::now();

    return duration_cast<nanoseconds>(stop - start);
}

void print_time_of_execution(nanoseconds total_time) {
    // 1 second = 1 000 000 000 ns
    cout << total_time.count() << "ns" << endl;
}

template <typename TElement>
void print_array(TElement* arr, int size) {
    cout << "[";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i + 1 != size)
            cout << ", ";
    }
    cout << "]" << endl;
}

template <typename TElement>
TElement* generate_array(int size, TElement(*generate)()) {
    TElement* buffer = new TElement[size];

    for (int i = 0; i < size; i++) {
        buffer[i] = generate();
    }

    return buffer;
}

float float_number_generator() {
    int precision = pow(10, RANDOM_PRECISION_NUMBERS);
    return rand() % (MAX_RANDOM_VALUE * precision) / (1.0 * precision);
}

bool float_ascending_comparer(float a, float b) {
    return a < b;
}

bool float_descending_comparer(float a, float b) {
    return a > b;
}

bool float_equal_comparer(float a, float b) {
    return a == b;
}

template <typename TElement>
void bubble_sort(TElement* arr, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    TElement buffer;

    for (int i = 0; i < sizeOfArray; i++)
        for (int j = 0; j < sizeOfArray - i - 1; j++)
            if (comparer(arr[j], arr[j + 1])) {
                buffer = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = buffer;
            }
}

template <typename TElement>
void insertion_sort(TElement* arr, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    int i, j;
    TElement key;
    for (i = 1; i < sizeOfArray; i++) {
        j = i;
        while (j > 0 && comparer(arr[j - 1], arr[j])) {
            key = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = key;
            j--;
        }
    }
}

template <typename TElement>
int index_of(TElement* arr, int startIndex, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    TElement minValue = arr[startIndex];
    int minIndex = startIndex;

    for (int i = minIndex + 1; i < sizeOfArray; i++) {
        if (comparer(arr[i], minValue)) {
            minIndex = i;
            minValue = arr[i];
        }
    }

    return minIndex;
}

template <typename TElement>
void selection_sort(TElement* arr, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    for (int i = 0; i < sizeOfArray; i++) {
        int index = index_of(arr, i, sizeOfArray, comparer);
        TElement temp = arr[i];
        arr[i] = arr[index];
        arr[index] = temp;
    }
}

template <typename TElement>
void merge(TElement* arr, int p, int q, int r, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    TElement* buffer = new TElement[sizeOfArray];
    int i, j, k;
    k = 0;
    i = p;
    j = q + 1;
    while (i <= q && j <= r) {
        if (comparer(arr[i], arr[j])) {
            buffer[k++] = arr[i++];
        }
        else {
            buffer[k++] = arr[j++];
        }
    }

    while (i <= q) {
        buffer[k++] = arr[i++];
    }

    while (j <= r) {
        buffer[k++] = arr[j++];
    }

    for (i = r; i >= p; i--) {
        arr[i] = buffer[--k];
    }

    delete buffer;
}

template <typename TElement>
void merge_sort(TElement* arr, int p, int r, int sizeOfArray, bool(*comparer)(TElement, TElement)) {
    int q;
    if (p < r) {
        q = (p + r) / 2;
        merge_sort(arr, p, q, sizeOfArray, comparer);
        merge_sort(arr, q + 1, r, sizeOfArray, comparer);
        merge(arr, p, q, r, sizeOfArray, comparer);
    }
}

template <typename TElement>
void swap(TElement* a, TElement* b)
{
    TElement buffer = *a;
    *a = *b;
    *b = buffer;
}

template <typename TElement>
int partition(TElement* arr, int low, int high, bool(*comparer)(TElement, TElement),
    bool(*equal)(TElement, TElement)) {
    TElement pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (comparer(arr[j], pivot) || equal(arr[j], pivot)) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}

template <typename TElement>
void quick_sort(TElement* arr, int low, int high, bool(*comparer)(TElement, TElement),
    bool(*equal)(TElement, TElement)) {
    if (low < high) {
        int pivot = partition(arr, low, high, comparer, equal);

        quick_sort(arr, low, pivot - 1, comparer, equal);
        quick_sort(arr, pivot + 1, high, comparer, equal);
    }
}

int main()
{
    int size = 15;
    float* arr = generate_array<float>(size, float_number_generator);

    print_array(arr, size);

    // merge_sort(arr, 0, size - 1, size, float_ascending_comparer);

    quick_sort<float>(arr, 0, size - 1, float_descending_comparer, float_equal_comparer);

    print_array(arr, size);


    return 0;
}

