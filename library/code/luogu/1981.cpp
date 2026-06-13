#include <bits/stdc++.h>
using namespace std;


int main(){
	int num, temp;
	char op;
	stack<int> addition;
	cin >> num;
	addition.push(num);
	while (cin >> op >> num){
		if (op == '+'){
			addition.push(num);
		}
		else{
			temp = addition.top();
			addition.pop();
			addition.push(temp*num%10000);
		}
	}
	int result = 0;
	while (addition.size() > 0){
		result += addition.top();
		addition.pop();
		result = result%10000;
	}
	cout << result;
}
