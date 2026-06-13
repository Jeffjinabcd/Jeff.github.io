#include <bits/stdc++.h>
using namespace std;

int main (){
	int N;
	cin >> N;
	string GH;
	cin >> GH;
	int G = 0;
	int H = 0;
	int result = 0;
	for (int i = 0; i < N - 2; ++i){
		G = 0;
		H = 0;
		/*
		if (GH[i] == 'G'){
			++G;
		}
		else{
			++H;
		}
		if (GH[i + 1] == 'G'){
			++G;
		}
		else{
			++H;
		}
		if (GH[i + 2] == 'G'){
			++G;
		}
		else{
			++H;
		}
		if (G == 1 || H == 1){
			++result;
		}
		*/
		for (int j = i; j < N; ++j){
			if (GH[j] == 'G'){
				++G;
			}
			else{
				++H;
			}
			if (G + H < 3){
				continue;
			}
			if (G == 1 || H == 1){
				++result;
			}
			else{
				break;
			}
		}
	}
	cout << result;
}
