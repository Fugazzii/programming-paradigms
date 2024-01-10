#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define Record pair<int, int>

vector<int> solution(vector<Record>& data, int& n) {
    vector<int> dp(n);
    dp[n - 1] = 1;

    for (int i = n - 2; i >= 0; i--) {
        int xi = data[i].first;
        int hi = data[i].second;
        int xNext = data[i + 1].first;

        if (xi + hi <= xNext) {
            dp[i] = i + 1;
        } else {
            int max = dp[i];
            for (int j = i + 1; j < n; j++) {
                if (max < dp[j]) {
                    max = dp[j];
                }
            }
            dp[i] = max;
        }
    }
    return dp;
}

int main() {

    int n = 4;

    vector<Record> data = {
        make_pair(16, 5),
        make_pair(20, 5),
        make_pair(10, 10),
        make_pair(18, 2)
    };

    auto result = solution(data, n);

    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;

    return 0;
}
