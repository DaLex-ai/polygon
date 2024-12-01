#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <algorithm> // для min и copy
using namespace std;

const int RUN = 100; // Размер "подмассива" для сортировки вставками

// Преобразование считанной строки в вектор
vector<int> stringToVector(const string& line) {
    vector<int> originalVector;
    stringstream ss(line);
    string item;

    while (getline(ss, item, ',')) {
        try {
            originalVector.push_back(stoi(item));
        } catch (invalid_argument&) {
            cerr << "Ошибка преобразования строки в число: " << item << endl;
        } catch (out_of_range&) {
            cerr << "Число вне диапазона: " << item << endl;
        }
    }
    return originalVector;
}

// Преобразование вектора в строку
string vectorToString(const vector<int>& vec) {
    ostringstream oss;
    for (int i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i < vec.size() - 1) {
            oss << ",";
        }
    }
    return oss.str();
}

// Функция для сортировки небольших массивов с помощью сортировки вставками
void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;

        // Перемещаем элементы arr[0..i-1], которые больше key, на одну позицию вперед
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Функция для слияния двух подмассивов
void merge(vector<int>& arr, int left, int mid, int right) {
    // Создаём временные массивы для хранения подмассивов
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;

    // Сливаем временные массивы обратно в arr
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // Копируем оставшиеся элементы, если есть
    while (i < leftArr.size()) {
        arr[k++] = leftArr[i++];
    }
    while (j < rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

// Основная функция Timsort
void timSort(vector<int>& arr) {
    int n = arr.size();

    // Сортируем подмассивы размером RUN
    for (int start = 0; start < n; start += RUN) {
        int end = min(start + RUN - 1, n - 1);
        insertionSort(arr, start, end);
    }

    // Сливаем отсортированные подмассивы
    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = min(left + size - 1, n - 1);
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}


// Функция для проверки ответа
bool comparePartitions(string lineTest, string lineAnswer) {
    if (lineTest.size() != lineAnswer.size()) return false;
    return lineTest == lineAnswer;
}

// Функция для логирования времени выполнения
void logExecutionTime(const chrono::duration<double>& elapsed, const string& outputFile) {
    ofstream outfile(outputFile, ios_base::app);
    if (outfile.is_open()) {
        outfile << elapsed.count() << endl;
    } else {
        cerr << "Не удалось открыть файл для записи времени!" << endl;
    }
}

// Функция для записи результирующего ответа в файл
void logOutput(const string& outputLine, const string& outputFile) {
    ofstream outfile(outputFile, ios_base::app);
    if (outfile.is_open()) {
        outfile << outputLine << endl;
    } else {
        cerr << "Не удалось открыть файл для записи ответа!" << endl;
    }
}

int main() {
    ifstream inputTest("numbers_1.csv");
    ifstream answerFile("answer_1.txt");

    if (!inputTest) {
        cerr << "Не удалось открыть файл тестов!" << endl;
        return 0;
    }

    // if (!answerFile) {
    //     cerr << "Не удалось открыть файл ответов!" << endl;
    //     return 0;
    // }

    string line;
    vector<int> unsortedVector;
    const int chunkSize = 1000;

    while (getline(inputTest, line)) {
        int start = 0;
        vector<int> originalVector = stringToVector(line);
        int sizeOriginalVector = originalVector.size();
        while (start < sizeOriginalVector) {
            int end = start + chunkSize;
            vector<int> subset(originalVector.begin() + end, originalVector.begin() + end + chunkSize);
            unsortedVector.insert(unsortedVector.end(), subset.begin(), subset.end());

            vector<int> sortedVector = unsortedVector;
            auto startTime = chrono::high_resolution_clock::now();
            timSort(sortedVector);
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            //logOutput(vectorToString(sortedVector), "program_outputs.txt");
            logExecutionTime(elapsed, "execution_times_tim_sort_1.txt");

            string sortedLine = vectorToString(sortedVector);

            // string expectedAnswer;
            // if (!getline(answerFile, expectedAnswer)) {
            //   cerr << "Ошибка: недостаточно строк в файле ответов." << endl;
            //    return 0;
            // }

            // if (comparePartitions(sortedLine, expectedAnswer)) {
            //     cout << "Ответ совпадает с ожидаемым!" << endl;
            // } else {
            //     cout << "Ответ НЕ совпадает с ожидаемым!" << endl;
            // }

            start = (end == sizeOriginalVector) ? end : end + 1;
        }
    }

    return 0;
}