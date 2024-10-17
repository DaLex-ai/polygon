#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

string current_line, res; // O(1) + O(1) - выделение памяти
int sizing = 0; // O(1) + O(1) - выделение памяти и присваивание
int number_lines = 0; // O(1) + O(1) - выделение памяти и присваивание
string *arr = new string[sizing]; // O(1) + O(1) - выделение памяти и присваивание
list<string> answer_test; // Для проверки тестов, в алгоритме не считаем
list<string> answer; // Для проверки тестов, в алгоритме не считаем


string mixing(int sizing, string current_line){
    arr = new string[sizing]; // O(1) - присвоение
    res = ""; // O(1) - присвоение
    for(int j = 0; j < sizing; j++){ // O(N) - в худшем случае, O(1) - когда sizing = 1, O(K) - средний вариант между O(1) и O(N)
        if(j != sizing - 1) // O(1) - проверка равенства
            arr[j] = current_line.substr(j+1, 1); // O(1) - присвоение
        else
            arr[j] = current_line.substr(0, 1); // O(1) - присвоение
        res += arr[j]; // O(1) - добавление элемента
    }
    delete[] arr; // O(1) - удаление
    return res; 
}

string test(list<string> answer_test){ // Для проверки тестов, в алгоритме не считаем сложность
    ifstream stream_answer("answer.txt");
    while (getline(stream_answer, current_line))
        answer.push_back(current_line);
    if(answer_test == answer)
        return "Рабочий код :)";
    else
        return "Где-то есть ошибка :(";
}

int main(){
    ifstream stream_test("test.txt");
    while (getline(stream_test, current_line)){ // Не учитываем данную сложность, так как это считывание файла для тестов
        number_lines += 1; // O(1) - увеличенние значения переменной
        if(number_lines % 2 != 0){ // O(1) - проверка деления переменной на 2
            sizing = stoi(current_line); // O(1) - присвоение
        }
        else if(sizing == current_line.length() && number_lines % 2 == 0){ // O(1) + O(1) - сравнение и проверка на деление на два
            cout << mixing(sizing, current_line) << endl;
            answer_test.push_back(mixing(sizing, current_line)); // Для проверки тестов, в алгоритме не считаем сложность
        }
        else{
            cout << "False" << endl;
            answer_test.push_back("False"); // Для проверки тестов, в алгоритме не считаем сложность
        }
    }
    cout << test(answer_test);
    // Сложность алгоритма: O(N) - в худшем, O(K) - в среднем, O(1) - в лучшем
}
