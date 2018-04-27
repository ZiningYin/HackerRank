//https://www.hackerrank.com/challenges/median/problem

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



void median(vector<char> s,vector<long> X) {
    multiset <long> intSet;
    multiset<long>::iterator it,tempIt;
    int setSize,length=s.size();
    cout << fixed;
    
    for (int i =0;i<length;++i){
        setSize = intSet.size();
        if (s[i]=='r'){
            if (setSize==0 || (tempIt=intSet.find(X[i]))==intSet.end()){
                cout<<"Wrong!\n";
                continue;
            }
            else if (X[i] == *it){
                tempIt=it;
                setSize % 2 ? --it : ++it;
                intSet.erase(tempIt);
            }
            else {
                intSet.erase(tempIt);
                if (X[i] < *it) //erase value on left
                    setSize % 2 ? it : ++it;
                else        //erase value on right
                    setSize % 2 ? --it : it;
            } 
            
            if (intSet.size()==0){
                //it=NULL; can't null an iterator, just don't use if set is 0
                cout<<"Wrong!\n";
                continue;
            }
        } else{ // insert
            intSet.insert(X[i]);
            if (setSize==0)
                it=intSet.begin();
            else if (X[i]>=*it) //insert right
                setSize % 2 ? it : ++it;
            else
                setSize % 2 ? --it : it;
        }
        double d = intSet.size() % 2 ? *it : (*it + *next(it))/2.0;
        if ((long)d==d){
            std::cout.precision(0);
            cout<<(long)d;
        }else {
            std::cout.precision(1);
            cout<<d;
        }
        cout << "\n";
    }
}

int main(void){
//Helpers for input and output
   int N;
   cin >> N;
   
   vector<char> s;
    vector<long> X;
   char temp;
    long tempint;
   for(int i = 0; i < N; i++){
      cin >> temp >> tempint;
        s.push_back(temp);
        X.push_back(tempint);
   }
   
   median(s,X);
   return 0;
}
