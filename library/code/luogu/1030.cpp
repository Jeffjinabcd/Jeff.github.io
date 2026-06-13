#include <bits/stdc++.h>
using namespace std;

void preOrder(string in,string post){
	if (in.length() > 0) {
        char root = post[post.length() - 1];
        putchar(root);
        int k = in.find(root);
        preOrder(in.substr(0, k), post.substr(0, k));
        preOrder(in.substr(k+1), post.substr(k, post.length() - k - 1));
    }
}

void postOrder(string pre, string in)
{
	if (pre.length() == 0) {
        return;
    }
    char root = pre[0];
    int k = in.find(root);
    postOrder(pre.substr(1, k), in.substr(0, k));
    postOrder(pre.substr(k + 1), in.substr(k + 1));
    putchar(root);
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	
    string inOrder, postOrder;
    cin>>inOrder>>postOrder;
    preOrder(inOrder, postOrder);
    cout<<endl;
    return 0;
}
