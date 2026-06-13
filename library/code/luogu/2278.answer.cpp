#include <bits/stdc++.h>
using namespace std;

struct process {
	// 进程号，到达时间，执行时间和优先级
	int num, s, t, p; 
};

vector<process> v;  // 还未开始的进程

struct cmp {     // 仿函数 
	bool operator()(process &a, process &b) {
		// 先按照优先级比较， 优先级相同，按先后顺序比较
		if(a.p!=b.p) {
			return a.p < b.p;
		}
		return a.num > b.num;
	}
};

priority_queue<process, vector<process>, cmp> pq;

int main() { 
	ios::sync_with_stdio(false);
	cin.tie(0);

	int num, s, t, p;
	while(cin>>num>>s>>t>>p) {
		v.push_back({num, s, t, p});
	}

	int rest = v.size();
	int cnt = 0;  // 下一个还未开始的进程下标
	int currentTime=0;
 	while(rest>0) {  // n个进程没有全部结束
		// cpu是否为空
		if(pq.empty()) {
			pq.push(v[cnt]);
			currentTime=v[cnt].s;
			cnt++;
		}
		process tmp = pq.top();
		pq.pop();
		int lastTime = currentTime;
		if(cnt<v.size()) {
			currentTime = min(v[cnt].s, lastTime + tmp.t);
		} else {
			currentTime = lastTime + tmp.t;
		}
		
		if(currentTime==lastTime+tmp.t) { // 该进程结束，输出结束时间
			printf("%d %d\n", tmp.num, currentTime);
			rest--;
		} else {
			// 时间结算
			tmp.t -= currentTime-lastTime;
			pq.push(tmp);
			
			// 加入新的进程
			pq.push(v[cnt]);
			cnt++;
		}
	}

	return 0;
} 
