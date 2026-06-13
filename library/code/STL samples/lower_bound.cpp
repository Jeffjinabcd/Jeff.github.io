#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
const int INF=INT_MAX;
#define LL long long

/*
lower_bound( )和upper_bound( )都是利用二分查找的方法在一个排好序的数组中进行查找的。
在从小到大的排序数组中，
lower_bound(begin,end,key)：从数组的begin位置到end-1位置二分查找第一个大于或等于
key的数字，找到返回该数字的地址，不存在则返回end。通过返回的地址减去起始地址begin,
得到找到数字在数组中的下标。

upper_bound(begin,end,key)：从数组的begin位置到end-1位置二分查找第一个大于key的
数字，找到返回该数字的地址，不存在则返回end。通过返回的地址减去起始地址begin,得到找
到数字在数组中的下标。

在从大到小的排序数组中，重载lower_bound()和upper_bound()
lower_bound( begin,end,key,greater<type>() ):从数组的begin位置到end-1位置二分查
找第一个小于或等于key的数字，找到返回该数字的地址，不存在则返回end。通过返回的地址减
去起始地址begin,得到找到数字在数组中的下标。

upper_bound( begin,end,key,greater<type>() ):从数组的begin位置到end-1位置二分查
找第一个小于key的数字，找到返回该数字的地址，不存在则返回end。通过返回的地址减去起始
地址begin,得到找到数字在数组中的下标。
*/
int main(){
	cout << "array usage:" << endl;
	int num[6]={1,2,4,7,15,34}; 
	sort(num,num+6);                          //按从小到大排序 
	int pos1=lower_bound(num,num+6,7)-num;    //返回数组中第一个大于或等于被查数的值 
	int pos2=upper_bound(num,num+6,7)-num;    //返回数组中第一个大于被查数的值
	cout<<pos1<<" "<<num[pos1]<<endl;
	cout<<pos2<<" "<<num[pos2]<<endl;
	sort(num,num+6, greater<int>());          //按从大到小排序
    // 34 15 7 4 2 1
	int pos3=lower_bound(num,num+6,7, greater<int>())-num;  //返回数组中第一个小于或等于被查数的值 
	int pos4=upper_bound(num,num+6,7, greater<int>())-num;  //返回数组中第一个小于被查数的值 
	cout<<pos3<<" "<<num[pos3]<<endl;
	cout<<pos4<<" "<<num[pos4]<<endl;

    cout<<"vector usage:"<<endl;
    vector<int> vec;
    for (int i = 0; i < 6; i++)
    {
        vec.push_back(num[i]);
    }
    sort(vec.begin(), vec.end());            //按从小到大排序 
     //返回数组中第一个大于或等于被查数的值 
	pos1=lower_bound(vec.begin(), vec.end(), 7)-vec.begin();
    //返回数组中第一个大于被查数的值
	pos2=upper_bound(vec.begin(), vec.end(), 7)-vec.begin();  
	cout<<pos1<<" "<<vec[pos1]<<endl;
	cout<<pos2<<" "<<vec[pos2]<<endl;

	return 0;
}