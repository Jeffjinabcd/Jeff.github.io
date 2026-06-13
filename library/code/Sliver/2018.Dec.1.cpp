#include <bits/stdc++.h>
using namespace std;

int N, M, C;
vector<int> lists;

bool validate(int k) {
	int result = 0;
	int current = 0;
	int first = 0;

	while (current < N) {
		if (current == first) {
			result++;
		}
		if (lists[current] - lists[first] > k) {
			first = current;
		}
		else if (current - first + 1 == C) {
			first = ++current;
		}
		else {
			current++;
		}
	}
	return result <= M;
}

int main() {
	freopen("convention.in", "r", stdin);
	freopen("convention.out", "w", stdout);

	int temp;
	cin >> N >> M >> C;
	lists.resize(N);
	for (int i = 0; i < N; i++) {
		cin >> lists[i];
	}
	sort(lists.begin(), lists.end());
	int low = 0;
	int high = lists[N - 1] - lists[0];

	while (low < high) {
		int mid = low + (high - low) / 2;

		if (validate(mid)) {
			high = mid;
		} else {
			low = mid + 1;
		}
	}
	cout << low;
}