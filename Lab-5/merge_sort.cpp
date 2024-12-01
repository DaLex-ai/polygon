#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;

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

// Функция для слияния двух подмассивов
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Основная функция сортировки слиянием
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Рекурсивная сортировка первых и вторых половин
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Слияние отсортированных половин
        merge(arr, left, mid, right);
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
            mergeSort(sortedVector, 0, sortedVector.size() - 1);
            auto endTime = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = endTime - startTime;

            //logOutput(vectorToString(sortedVector), "program_outputs.txt");
            logExecutionTime(elapsed, "execution_times_merge_sort_1.txt");

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