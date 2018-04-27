#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
/* Head ends here */


typedef struct node
{
    int val;
    struct node* left;
    struct node* right;
    int ht;
} node; 

int getBalance (node *root);
int getHeight (node *root);
node * balanceRight (node *root);
node * balanceLeft (node *root);
node * insert(node * root,int val);
node * del (node * root,int val);
float findMedian (node * root);

void median(vector<char> s,vector<int> X) {
    node * root=NULL;
    for (int i =0;i<s.size();i++){
        if (s[i]=='r'){
            root=del(root,X[i]);
            if (root==NULL){
                cout<<"Wrong!\n";
                continue;
            }
        } else{
            root=insert(root,X[i]);
        }
    }
}

int main(void){
//Helpers for input and output
   int N;
   cin >> N;
   
   vector<char> s;
    vector<int> X;
   char temp;
    int tempint;
   for(int i = 0; i < N; i++){
      cin >> temp >> tempint;
        s.push_back(temp);
        X.push_back(tempint);
   }
   
   median(s,X);
   return 0;
}


//delete might have some bugs
node * del (node * root,int val){
    if (root==NULL)
        return root;
    
    if (val>root->val){
        root->right=del(root->right,val); //left side is heavy
        //cout<<root->val<<getBalance(root)<<getHeight(root)<<" ";
        if (getBalance(root)>1)
            root=balanceLeft(root);
        else
            root->ht=getHeight(root);   
    }
    else if (val<root->val)  {
        root->left=del(root->left,val);
        if (getBalance(root)<-1) // right side is heavy
            root=balanceRight(root);
        else
            root->ht=getHeight(root);
    } else{ //the node to delete is found, now deleting
        if (root->left == NULL && root->right == NULL){
            delete root;
            return NULL;
        }else if (root->left== NULL){
            node * temp = root->right;
            delete root;
            return temp;
        }else if (root->right== NULL){
            node * temp = root->left;
            delete root;
            return temp;
        }else{
            int bal = getBalance (root);
            node * temp;
            int tempVal;
            if (bal>0) { // left heavy
                temp= root->left;
                while (temp->right != NULL){
                    temp=temp->right;
                }
                tempVal = temp->val;
                root->left=del(root->left,tempVal);
                if (getBalance(root)<-1) // right side is heavy
                    root=balanceRight(root);
                else
                    root->ht=getHeight(root);
            } else { // right heavy or balanced
                temp= root->right;
                while (temp->left != NULL){
                    temp=temp->left;
                }
                tempVal = temp->val;
                root->right=del(root->right,tempVal);
                if (getBalance(root)>1)
                    root=balanceLeft(root);
                else
                    root->ht=getHeight(root);
            }
            root->val = tempVal;
            return root;
        }
        

    }
    return root;
}


node * insert(node * root,int val)
{
    if (root==NULL){
        root = new node (); root->ht=0; root->val=val; root->left=NULL; root->right=NULL;
        return root;
    }
    if (val>root->val){
        root->right=insert(root->right,val);
        //cout<<root->val<<getBalance(root)<<getHeight(root)<<" ";
        if (getBalance(root)<-1)
            root=balanceRight(root);
        else
            root->ht=getHeight(root);
    }
    else if (val<root->val) {
        root->left=insert(root->left,val);
        if (getBalance(root)>1)
            root=balanceLeft(root);
        else
            root->ht=getHeight(root);   
    } else {
        if (getBalance(root)<0){
            root->left=insert(root->left,val);
            if (getBalance(root)>1)
                root=balanceLeft(root);
            else
                root->ht=getHeight(root);  
        } else{
            root->right=insert(root->right,val);
            if (getBalance(root)<-1)
                root=balanceRight(root);
            else
                root->ht=getHeight(root);
        }
    }
    return root;
}

node * balanceRight (node *root){
    node *one,*two,*three;
    one=root;
    two=root->right;
    if (getBalance (two)>0){
        //right-left case
        three=two->left;
        one->right = three->left;
        two->left=three->right;
        three->left=one;
        three->right=two;
        one->ht=getHeight(one);
        two->ht=getHeight(two);
        three->ht=getHeight(three);
        return three;
    } else{
        //right right case
        three=two->right;
        one->right=two->left;
        two->left=one;
        one->ht=getHeight(one);
        three->ht=getHeight(three);
        two->ht=getHeight(two);
        return two;
    }
    return root;
}

node * balanceLeft (node *root){
    node *one,*two,*three;
    one=root;
    two=root->left;
    if (getBalance (root->left)<0){
        //left-right case
        three=two->right;
        two->right = three->left;
        one->left=three->right;
        three->left=two;
        three->right=one;
        one->ht=getHeight(one);
        two->ht=getHeight(two);
        three->ht=getHeight(three);
        return three;
    } else{
        //left left case
        three=two->left;
        one->left=two->right;
        two->right=one;
        one->ht=getHeight(one);
        three->ht=getHeight(three);
        two->ht=getHeight(two);
        return two;
    }
    return root;
}

int getBalance (node *root){
    int lHt = root->left == NULL ? -1 : root->left->ht;
    int rHt = root->right == NULL ? -1 : root->right->ht;
    return lHt-rHt;
}
int getHeight (node *root){
    int lHt = root->left == NULL ? -1 : root->left->ht;
    int rHt = root->right == NULL ? -1 : root->right->ht;
    return lHt>rHt?lHt+1:rHt+1;
}
