#include <bits/stdc++.h>
using namespace std;

const int MAX_T = 1e5 + 5;
const int MAX_C = 1e5 + 5;

int T, C, ans;
int targetPositions[MAX_T], pos[MAX_T];
int leftVal[MAX_T], rightVal[MAX_T][5];
int book[5][MAX_T];
char commands[MAX_C];

int commandValue(char c) {
    if(c == 'F') return 0;
    return c == 'L' ? -1 : 1;
}

void init() {
    pos[0] = C;
    for (int i = 1; i <= C; i++) {
        pos[i] = pos[i - 1];
        if (commands[i] == 'L')
            pos[i]--;
        else if (commands[i] == 'R')
            pos[i]++;
    }

    for (int i = C; i; i--)
        if (commands[i] == 'F') {
            int j = lower_bound(targetPositions + 1, targetPositions + T + 1, pos[i]) - targetPositions;
            if (j <= T && targetPositions[j] == pos[i]) {
                leftVal[j] = i;  // j : target pos   i: command id
            }
        }

    for (int i = 1; i <= C; i++) {
        if (commands[i] == 'F') {
            for (int j = -2; j <= 2; j++) {
                int k = lower_bound(targetPositions + 1, targetPositions + T + 1, pos[i] + j) - targetPositions;
                if (k <= T && targetPositions[k] == pos[i] + j) {
                    rightVal[k][j + 2] = i;
                }
            }
        }
    }
}

void calBookArray() {
    for (int i = 1; i <= T; i++)
        for (int j = -2; j <= 2; j++) {
            if (leftVal[i] && rightVal[i][j + 2]) {
                if (leftVal[i] >= rightVal[i][j + 2]) {
                    book[j + 2][1]++;
                    book[j + 2][rightVal[i][j + 2]]--;
                    book[j + 2][leftVal[i] + 1]++;
                } else
                    book[j + 2][1]++;
            } else if (leftVal[i]) {
                book[j + 2][leftVal[i] + 1]++;
            } else if (rightVal[i][j + 2]) {
                book[j + 2][1]++;
                book[j + 2][rightVal[i][j + 2]]--;
            }
        }

    for (int j = 0; j <= 4; j++)
        for (int i = 2; i <= C; i++) {
            book[j][i] += book[j][i - 1];
        }
}

int main() {
    cin >> T >> C;

    for (int i = 1; i <= T; i++) {
        cin >> targetPositions[i];
        targetPositions[i] += C;
    }

    scanf("%s", commands + 1);
    sort(targetPositions + 1, targetPositions + T + 1);

    init();

    calBookArray();

    string targets = "LRF";
    for (int i = 1; i <= C; i++)
        for (int j = 0; j < 3; j++) {
            int shift = commandValue(targets[j]) - commandValue(commands[i]) + 2;
            int cnt = book[shift][i];

            if (targets[j] == 'F') {
                int k = lower_bound(targetPositions + 1, targetPositions + T + 1, pos[i - 1]) - targetPositions;
                if (k <= T && targetPositions[k] == pos[i - 1] &&
                    !(leftVal[k] && leftVal[k] < i) && !(rightVal[k][shift] && rightVal[k][shift] > i))
                    cnt++;
            }

            ans = max(ans, cnt);
        }

    cout << ans << endl;

    return 0;
}
