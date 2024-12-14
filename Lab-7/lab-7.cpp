#include <iostream>
#include <vector>
#include <sstream>  // Для использования istringstream
using namespace std;

vector<int> stringToVector(const string& rods) {
    string line = rods;
    
    // Удаляем скобки из строки
    line.erase(0, 1);
    line.erase(line.length() - 1, 1);

    vector<int> vectorRods;
    string element;
    istringstream ss(line);  // Используем istringstream для разбора строки

    // Разбиваем строку по запятым
    while (getline(ss, element, ',')) {
        vectorRods.push_back(stoi(element)); // Преобразуем в целое число и добавляем в вектор
    }

    return vectorRods;
}

class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> candies(n, 1);

        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }

        for (int i = n - 2; i >= 0; i--) {
            if (ratings[i] > ratings[i + 1]) {
                candies[i] = max(candies[i], candies[i + 1] + 1);
            }
        }

        int totalCandies = 0;
        for (int candy : candies) {
            totalCandies += candy;
        }

        return totalCandies;
    }
};

int main() {
    string line = "";
    cout << "Введите массив рейтингов (например, [1,0,2]): ";
    cin >> line;
    vector<int> vector_line = stringToVector(line);
    
    Solution solution;
    int result = solution.candy(vector_line);
    
    cout << "Общее количество конфет: " << result << endl;

    return 0;
}