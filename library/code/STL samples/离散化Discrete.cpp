#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
int a[MAXN], b[MAXN];

// 离散化，就是把无限空间中有限的个体映射到有限的空间中去，以此提高算法的时空效率。
// 通俗来讲，就是在不改变数据相对大小的条件下，对数据进行相应的压缩。
// input: 
// 8
// 1 3377 939447 0 47 3 37 1
//
// 排序并去重后 0 1 3 37 47 3377 939447
// 利用lower_bound 算出离散化后的排列
//
// output: 1 5 6 0 4 2 3 1
int	main()
{    
    int n;
	cin>>n;
	for(int i=0; i<n; i++) {
		cin>>a[i];
		b[i] = a[i];
	}
	sort(a, a+n);
	int len = unique(a, a+n) - a;  // unique在有序数组中去除重复数据
	for(int i = 0; i < n; i++) {
		b[i] = lower_bound(a, a+len, b[i]) - a;
		cout << b[i] << " ";
	}

	return 0;
}
