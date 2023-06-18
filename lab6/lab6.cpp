#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <synchapi.h>

using namespace std;

mutex mut;
condition_variable cv;
int n;
int k;
int* finalArray;
double sumResult = 0;

void work(char* array) {
    mut.lock();
    int timeInterval;
    cout << "Enter the time to sleep between elements: ";
    cin >> timeInterval;

    finalArray = new int[n];
    int finalArraySize = 0;
    for (int i = 0; i < n; i++) {
        if (isdigit(array[i])) {
            finalArray[finalArraySize] = array[i] - '0';
            finalArraySize++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
    }

    if (finalArraySize < k) {
        k = finalArraySize;
    }

    for (int i = 0; i < k; i++) {
        cout << finalArray[i] << " ";
    }
    cout << '\n';
    mut.unlock();
}

void CountElement(int* array) {
    mut.lock();
    for (int i = 0; i < k; i++) {
        sumResult += finalArray[i];
    }

    cout << "Sum result: " << sumResult << endl;
    sumResult /= k;

    cv.notify_one();
    mut.unlock();
}

int main() {
    cout << "Enter size of the array: ";
    cin >> n;
    auto array = make_unique<char[]>(n);
    cout << "Enter elements of the array: ";
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }

    cout << "Array size is " << n << '\n';
    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }

    cout << "\nEnter k: ";
    cin >> k;

    thread t1(work, array.get());
    thread t2(CountElement, finalArray);

    unique_lock<mutex> lock(mut);

    cv.wait(lock);

    cout << "Arithmetic mean up to position k: " << sumResult << endl;

    t1.join();
    t2.join();
    return 0;
}
