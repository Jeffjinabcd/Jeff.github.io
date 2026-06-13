#include <bits/stdc++.h>
using namespace std;

const int N = 500001;
const int M = 500001;

struct edge {
	int to; 
	int index;
};

vector<int> g[N];
vector<edge> gq[N];

int n,m,s;

int fa[N], lca[M];
bool vis[N];

int find(int u) {
	if(u==fa[u])
		return u;
	
	fa[u]=find(fa[u]); 
	return fa[u];
}

void Tarjan(int u) {
	
	vis[u]=true;
	// 访问u的所有子节点e.to
    // TODO
	for (int to : g[u]) {
		if (vis[to]) {
			continue;
		}
		Tarjan(to);
		fa[to] = u;
	}
	//访问所有和u有询问关系的点
    // TODO
	for (edge e : gq[u]) {
		if (vis[e.to]) {
			lca[e.index] = find(e.to);
		}
	}
}

/*
Tarjan(离线)算法时间复杂度是O(n+q)
在一次遍历中把所有询问一次性解决，所以其时间复杂度是O(n+q)

基本思路：
1.任选一个点为根节点，从根节点开始。
2.遍历该点u所有子节点v，并标记这些子节点v已被访问过。
3.若是v还有子节点，返回2，否则下一步。
4.合并v到u上。
5.寻找与当前点u有询问关系的点v。
6.若是v已经被访问过了，则可以确认u和v的最近公共祖先为v被合并到的父亲节点a。

利用并查集合并两个点
*/
int main() {
	scanf("%d%d%d",&n,&m,&s);
	for(int i=1; i<=n; i++) {
		fa[i]=i;
	}
	
	for(int i=1; i<=n-1; i++) {
		int x, y;
		scanf("%d%d",&x,&y);
		g[x].push_back(y);
		g[y].push_back(x);
	}
	
	for(int i=1; i<=m; i++) {
		int x, y;
		scanf("%d%d",&x,&y);
		gq[x].push_back({y, i});
		gq[y].push_back({x, i});
	}
	
	Tarjan(s);
	
	for(int i=1; i<=m; i++) {
		printf("%d\n", lca[i]);
	}
	return 0;
}

