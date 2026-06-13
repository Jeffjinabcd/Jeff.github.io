#include <bits/stdc++.h>
using namespace std;


int main(){
	freopen("citystate.in", "r", stdin);
	freopen("citystate.out", "w", stdout);
	int N;
	cin >> N;
	string temp;
	map<string, int> CIST;
	string city;
	string State;
	for (int i = 0; i < N; ++i){
		cin >> city >> State;
		if (city.substr(0, 2) != State){
			CIST[city.substr(0, 2) + State]++;
		}
	}
	int result = 0;
	for (auto &kv : CIST) {
		temp = kv.first.substr(2, 2) + kv.first.substr(0, 2);
		result += CIST[temp] * kv.second;
	}
	cout << result / 2;
	
}
