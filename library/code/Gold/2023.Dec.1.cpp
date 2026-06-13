#include <bits/stdc++.h>
using namespace std;

const int MAXN = 751;

bool isOdd[MAXN][MAXN];
bool edge[MAXN][MAXN];

void readGraph(int n) {
	for (int i = 1; i < n; ++i) {
		for (int j = i + 1; j <= n; ++j) {
			char c;
			cin >> c;
			isOdd[i][j] = c  - '0';
			edge[i][j] = isOdd[i][j];
		}
	}
}

int countOddEdges(int n) {
	int oddEdgeCount = 0;
	
	
	for (int i = 1; i < n; ++i) {
		for (int j = i + 1; j <= n; ++j) {
			for (int k = i + 1; k < j; ++k) {
				edge[i][j] -= (edge[i][k] * isOdd[k][j]);
				edge[i][j] = (edge[i][j] + 2) % 2;
			}
			if (edge[i][j]) {
				oddEdgeCount++;
			}
		}
	}
	
	return oddEdgeCount;
}

int main() {
	int nodeCount;
	cin >> nodeCount;
	
	readGraph(nodeCount);
	int result = countOddEdges(nodeCount);
	
	cout << result << '\n';
	
	return 0;
}

