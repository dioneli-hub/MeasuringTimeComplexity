#include <chrono>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using namespace std::chrono;


const int MAX_RANDOM_VALUE = 1000;
const int RANDOM_PRECISION_NUMBERS = 2;
const int ARRAY_SIZE = 15;
const int COUNT_OF_TESTS = 6;

template <typename TElement>
TElement* generate_array(int size, TElement(*generate)()) {
    TElement* buffer = new TElement[size];

    for (int i = 0; i < size; i++) {
        buffer[i] = generate();
    }

    return buffer;
}

int int_number_generator(int max) {
    return rand() % max;
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

bool float_greather_than(float a, float b) {
    return a > b;
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

    delete[] buffer;
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

/*
template <typename TElement>
void quick_sort(TElement* arr, int low, int high, bool(*comparer)(TElement, TElement),
    bool(*equal)(TElement, TElement)) {
    if (low < high) {
        int pivot = partition(arr, low, high, comparer, equal);

        quick_sort(arr, low, pivot - 1, comparer, equal);
        quick_sort(arr, pivot + 1, high, comparer, equal);
    }
}
*/

struct Range {
    int start;
    int end;

    Range(int newStart, int newEnd) : start(newStart), end(newEnd) {}
};

template <typename TElement>
void quick_sort(TElement* arr, int start, int end, bool(*comparer)(TElement, TElement),
    bool(*equal)(TElement, TElement))
{
    queue<Range>  queue;
    queue.push(Range(start, end));

    while (!queue.empty())
    {
        Range next = queue.front();
        queue.pop();

        int start = next.start;
        int end = next.end;
        int wall = start;

        if (start < end)
        {
            for (int i = start; i < end; i++)
            {
                if (comparer(arr[i], arr[end]) || equal(arr[i], arr[end]))
                {
                    swap(&arr[wall], &arr[i]);
                    wall++;
                }
            }
            swap(&arr[end], &arr[wall]);
            queue.push(Range(start, --wall));
            queue.push(Range(++wall, end));
        }
    }
}


template <typename TElement>
int linear_search(TElement* arr, TElement target, int sizeOfArray, bool(*equal)(TElement, TElement)) {
    for (int i = 0; i < sizeOfArray; i++) {
        if (equal(arr[i], target)) {
            return i;
        }
    }

    return -1;
}

template <typename TElement>
int binary_search(TElement* arr, int len, TElement target, bool(*equal)(TElement, TElement),
    bool(*greather_than)(TElement, TElement)) {
    int max = (len - 1);
    int min = 0;

    int guess;
    int step = 0;

    while (max >= min) {
        guess = (max + min) / 2;
        step++;

        if (equal(arr[guess], target)) {
            return guess;
        }
        else {
            if (greather_than(arr[guess], target)) {
                max = (guess - 1);
            }
            else {
                min = (guess + 1);
            }
        }
    }

    return -1;
}

void demo_linear_search() {
    cout << "\t >>>>>> LINEAR SEARCH DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        //int random_index = int_number_generator(sizeOfArray);
        int random_index = sizeOfArray/2;

        cout << endl
            << "Linear search. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random index: " << random_index << endl
            << "\t-search value: " << arr[random_index] << endl;

        steady_clock::time_point start = high_resolution_clock::now();
        int search_index = linear_search<float>(arr, arr[random_index], sizeOfArray, float_equal_comparer);
        steady_clock::time_point stop = high_resolution_clock::now();
        nanoseconds execution_time = duration_cast<nanoseconds>(stop - start);

        cout << "\t-linear search index: " << search_index << endl
            << "\t-execution time: " << execution_time.count() << " nanoseconds" << endl
            << endl;
    }
}

void demo_binary_search() {
    cout << "\t >>>>>> BINARY SEARCH DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        quick_sort<float>(arr, 0, sizeOfArray - 1, float_ascending_comparer, float_equal_comparer);

        int random_index = int_number_generator(sizeOfArray);

        cout << endl
            << "Binary search. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random index: " << random_index << endl
            << "\t-search value: " << arr[random_index] << endl;

        steady_clock::time_point start = high_resolution_clock::now();
        int search_index = binary_search<float>(arr, sizeOfArray, arr[random_index], float_equal_comparer, float_greather_than);
        steady_clock::time_point stop = high_resolution_clock::now();
        nanoseconds execution_time = duration_cast<nanoseconds>(stop - start);

        cout << "\t-binary search index: " << search_index << endl
            << "\t-execution time: " << execution_time.count() << " nanoseconds" << endl
            << endl;
    }
}

void demo_bubble_sort() {
    cout << "\t >>>>>> BUBBLE SORT DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        steady_clock::time_point random_start = high_resolution_clock::now();
        bubble_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point random_stop = high_resolution_clock::now();

        steady_clock::time_point sorted_start = high_resolution_clock::now();
        bubble_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point sorted_stop = high_resolution_clock::now();

        bubble_sort<float>(arr, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point reversed_start = high_resolution_clock::now();
        bubble_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point reversed_stop = high_resolution_clock::now();


        cout << endl
            << "Bubble sort. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random array sort time: " << duration_cast<nanoseconds>(random_stop - random_start).count() << " nanoseconds" << endl
            << "\t-sorted array sort time: " << duration_cast<nanoseconds>(sorted_stop - sorted_start).count() << " nanoseconds" << endl
            << "\t-reversed array sort time: " << duration_cast<nanoseconds>(reversed_stop - reversed_start).count() << " nanoseconds" << endl
            << endl;
    }
}

void demo_insertion_sort() {
    cout << "\t >>>>>> INSERTION SORT DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        steady_clock::time_point random_start = high_resolution_clock::now();
        insertion_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point random_stop = high_resolution_clock::now();

        steady_clock::time_point sorted_start = high_resolution_clock::now();
        insertion_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point sorted_stop = high_resolution_clock::now();

        insertion_sort<float>(arr, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point reversed_start = high_resolution_clock::now();
        insertion_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point reversed_stop = high_resolution_clock::now();

        cout << endl
            << "Insertion sort. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random array sort time: " << duration_cast<nanoseconds>(random_stop - random_start).count() << " nanoseconds" << endl
            << "\t-sorted array sort time: " << duration_cast<nanoseconds>(sorted_stop - sorted_start).count() << " nanoseconds" << endl
            << "\t-reversed array sort time: " << duration_cast<nanoseconds>(reversed_stop - reversed_start).count() << " nanoseconds" << endl
            << endl;
    }
}

void demo_selection_sort() {
    cout << "\t >>>>>> SELECTION SORT DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        steady_clock::time_point random_start = high_resolution_clock::now();
        selection_sort<float>(arr, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point random_stop = high_resolution_clock::now();

        steady_clock::time_point sorted_start = high_resolution_clock::now();
        selection_sort<float>(arr, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point sorted_stop = high_resolution_clock::now();

        selection_sort<float>(arr, sizeOfArray, float_descending_comparer);
        steady_clock::time_point reversed_start = high_resolution_clock::now();
        selection_sort<float>(arr, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point reversed_stop = high_resolution_clock::now();


        cout << endl
            << "Selection sort. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random array sort time: " << duration_cast<nanoseconds>(random_stop - random_start).count() << " nanoseconds" << endl
            << "\t-sorted array sort time: " << duration_cast<nanoseconds>(sorted_stop - sorted_start).count() << " nanoseconds" << endl
            << "\t-reversed array sort time: " << duration_cast<nanoseconds>(reversed_stop - reversed_start).count() << " nanoseconds" << endl
            << endl;
    }
}

void demo_quick_sort() {
    cout << "\t >>>>>> QUICK SORT DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        steady_clock::time_point random_start = high_resolution_clock::now();
        quick_sort<float>(arr, 0, sizeOfArray - 1, float_ascending_comparer, float_equal_comparer);
        steady_clock::time_point random_stop = high_resolution_clock::now();

        steady_clock::time_point sorted_start = high_resolution_clock::now();
        quick_sort<float>(arr, 0, sizeOfArray - 1, float_ascending_comparer, float_equal_comparer);
        steady_clock::time_point sorted_stop = high_resolution_clock::now();

        quick_sort<float>(arr, 0, sizeOfArray - 1, float_descending_comparer, float_equal_comparer);
        steady_clock::time_point reversed_start = high_resolution_clock::now();
        quick_sort<float>(arr, 0, sizeOfArray - 1, float_ascending_comparer, float_equal_comparer);
        steady_clock::time_point reversed_stop = high_resolution_clock::now();

        delete[] arr;

        cout << endl
            << "Quick sort. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-random array sort time: " << duration_cast<nanoseconds>(random_stop - random_start).count() << " nanoseconds" << endl
            << "\t-sorted array sort time: " << duration_cast<nanoseconds>(sorted_stop - sorted_start).count() << " nanoseconds" << endl
            << "\t-reversed array sort time: " << duration_cast<nanoseconds>(reversed_stop - reversed_start).count() << " nanoseconds" << endl
            << endl;

        
    }
}

void demo_merge_sort() {
    cout << "\t >>>>>> MERGE SORT DEMO <<<<<<" << endl;
    for (int i = 0; i < COUNT_OF_TESTS; i++) {
        int sizeOfArray = (i + 1) * ARRAY_SIZE;
        float* arr = generate_array<float>(sizeOfArray, float_number_generator);

        steady_clock::time_point random_start = high_resolution_clock::now();
        merge_sort<float>(arr, 0, sizeOfArray - 1, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point random_stop = high_resolution_clock::now();

        steady_clock::time_point sorted_start = high_resolution_clock::now();
        merge_sort<float>(arr, 0, sizeOfArray - 1, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point sorted_stop = high_resolution_clock::now();

        merge_sort<float>(arr, 0, sizeOfArray - 1, sizeOfArray, float_descending_comparer);
        steady_clock::time_point reversed_start = high_resolution_clock::now();
        merge_sort<float>(arr, 0, sizeOfArray - 1, sizeOfArray, float_ascending_comparer);
        steady_clock::time_point reversed_stop = high_resolution_clock::now();


        cout << endl
            << "Merge sort. Test #" << i + 1 << "." << endl
            << "\t-size of array: " << sizeOfArray << endl
            << "\t-radom array sort time: " << duration_cast<nanoseconds>(random_stop - random_start).count() << " nanoseconds" << endl
            << "\t-sorted array sort time: " << duration_cast<nanoseconds>(sorted_stop - sorted_start).count() << " nanoseconds" << endl
            << "\t-reversed array sort time: " << duration_cast<nanoseconds>(reversed_stop - reversed_start).count() << " nanoseconds" << endl
            << endl;
    }
}

int main()
{

    //demo_bubble_sort();
    //demo_merge_sort();
    //demo_binary_search();
    //demo_quick_sort();
    //demo_linear_search();
    demo_quick_sort();

    /*
    * demo_linear_search();
    * demo_binary_search();
    * 
    demo_bubble_sort();
    demo_insertion_sort();
    demo_selection_sort();

    demo_quick_sort();
    demo_merge_sort();

    */
    
    return 0;
}


