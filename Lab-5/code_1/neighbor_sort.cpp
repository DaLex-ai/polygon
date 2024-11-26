#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;

//Преобразование считанной строки в вектор
vector<int> stringToVector(const string& line) {
    vector<int> originalVector;
    string cleanedLine = line;
    
    // Удаляем квадратные скобки
    cleanedLine.erase(0, 1);
    cleanedLine.erase(cleanedLine.size() - 1);

    //Считывание значений и запись их в вектор
    int num = 0;
    stringstream ss(cleanedLine);
    string item;
    while (getline(ss, item, ',')) {
        // Преобразуем каждое число в целое и добавляем в вектор
        originalVector.push_back(stoi(item));
    }

    return originalVector;
}

//Преобразование вектора в строку
string vectorToString(const vector<int>& vec) {
    ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i]; // Добавляем элемент в строку
        if (i < vec.size() - 1) {
            oss << ", "; // Добавляем запятую и пробел для всех, кроме последнего элемента
        }
    }

    oss << "]";
    return oss.str();
}

//Алгоритм сортировки
void neighborSort(vector<int>& arr) {
    bool sorted = false;
    int sizing = arr.size();
    
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < sizing - 1; ++i) {
            if (arr[i] > arr[i + 1]) { // сравниваем с правым соседом
                swap(arr[i], arr[i + 1]); // меняем местами
                sorted = false; // если есть перестановка, продолжаем сортировку
            }
        }
    }
}

// Функция для проверки ответа
bool comparePartitions(string lineTest, string lineAnswer) {
    if (lineTest.size() != lineAnswer.size()) return false;
    for (int i = 0; i < lineTest.size(); ++i) {
        if (lineTest[i] != lineAnswer[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    ifstream inputTest("test.txt");
    ifstream answerFile("answer.txt");

    if (!inputTest) {
        cerr << "Не удалось открыть файл!" << endl;
        return 0;
    }

    string line;
    while (getline(inputTest, line)) {
        auto start = chrono::high_resolution_clock::now();

        vector<int> originalVector;
        originalVector = stringToVector(line);
        int sizeVector = originalVector.size();

        neighborSort(originalVector);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        line = vectorToString(originalVector);
        
        cout << "Результат разбиения строки, содержащей " << to_string(sizeVector) << " элементов: "<< line << endl;
        cout << "Время выполнения: " << elapsed.count() << " секунд" << endl;

        // Читаем ожидаемое разбиение
        vector<int> expectedPartition;
        string answerLine;

        if (getline(answerFile, answerLine)) {
            stringstream ss(answerLine);
            string clusterLine;
            while (getline(ss, clusterLine)) { 
                stringToVector(clusterLine);
            }
        }

        // Сравниваем ответ с ожидаемым
        if (comparePartitions(line, answerLine)) {
            cout << "Ответ совпадает с ожидаемым." << endl;
        } else {
            cout << "Ответ НЕ совпадает с ожидаемым!" << endl;
        }
    }
}