#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<int> stringToVector(const string&  rods){

    string line = rods;
    line.erase(0, 1);
    line.erase(line.length() - 1, 1);

    vector<int> vectorRods;
    stringstream ss(line);
    string element;

    while (getline(ss, element, ',')) {
        vectorRods.push_back(stoi(element));
    }
    sort(vectorRods.begin(), vectorRods.end());

    return vectorRods;
}

int tallestBillboard(vector<int>& vectorRods){
    unordered_map<int, int> dp;
    dp[0] = 0;

    for (int rod : vectorRods) {
        unordered_map<int, int> cur = dp; 
        for (const auto& [sum, height] : dp) {
            cur[sum + rod] = max(cur[sum + rod], height + rod);
            cur[sum] = max(cur[sum], height);
            cur[sum - rod] = max(cur[sum - rod], height);
        }
        
        dp = cur;
    }

    return dp[0];
}

int main(){
    string rods;
    vector<int> vectorRods;

    cin >> rods;
    vectorRods = stringToVector(rods);
    cout << tallestBillboard(vectorRods);

    return 0;
}