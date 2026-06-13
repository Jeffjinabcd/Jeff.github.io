#include <bits/stdc++.h>
using namespace std;

struct node{
	int id,start,time,rank;
};

struct cmp_id{
    bool operator()(node &x, node &y) {
        return x.id > y.id;
    }
};

struct cmp_rank{
    bool operator()(node &x, node &y) {
        if (x.rank == y.rank){
            return x.start > y.start;
        }
        return x.rank < y.rank;
    }
};

int main(){
    // freopen("testing.in", "r", stdin);
    int temp1, temp2, temp3, temp4;
    node temp;
    priority_queue<node, vector<node>, cmp_id> lists;
    priority_queue<node, vector<node>, cmp_rank> waiting;
    while (cin >> temp1 >> temp2 >> temp3 >> temp4){
        temp = {temp1, temp2, temp3, temp4};
        lists.push(temp);
    }
    int amount = lists.size();
    temp = lists.top();
    lists.pop();
    map<int, int> result;
    int current = temp.start, previous = current, r;
    while (result.size() < amount){
        r = current + temp.time;
        if (r < lists.top().start){
            // cout << current << "  " << temp.time << "  " << temp.id << endl;
            result[r] += temp.id;
            if (waiting.size() == 0){
                waiting.push(lists.top());
                temp = waiting.top();
                current = max(temp.start, r);
                lists.pop();
                waiting.pop();
            }
            else {
                temp = waiting.top();
                waiting.pop();
                current = r;
            }
            
        }
        else if (r >= lists.top().start){
            current = lists.top().start;
            if (temp.rank >= lists.top().rank){
                temp.time -= current - previous;
                waiting.push(lists.top());
                lists.pop();
                // cout << "staying" << endl;
            }
            else{
                waiting.push({temp.id, temp.start, temp.time - current + previous, temp.rank});
                temp = lists.top();
                lists.pop();
                // cout << "idk" << endl;
            }
        }
        previous = current;
        // cout << current << endl;
        // for (auto &k : result){
        //     cout << k.second << " " << k.first << endl;
        // }
        // if (lists.size() != 0){
        //     cout << lists.top().id;
        // }
        // if (waiting.size() != 0){
        //     cout << "\t" << waiting.top().id;
        // }
        // cout << endl << "------" << endl << endl;
    }
    for (auto &k : result){
        cout << k.second << " " << k.first << endl;
    }
}