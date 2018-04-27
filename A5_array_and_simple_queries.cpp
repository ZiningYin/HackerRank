//https://www.hackerrank.com/challenges/array-and-simple-queries/problem

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/*
Used an array of nodes and a linked list to solve this problem. The most common solution that was suggested is a treap
I used a linked list, each node had the index of array it points to, and the length it continues for.
The nodes are a temporary list which is maintained by referencing the original list.

    For X nodes, each operation takes O(X) time. Very frequently, every 10-15 operations, the nodes will be consolidated into a single node,
and the array is remade, so each operation is O(1).
    However, The consolidated should take O(N) time, N being the length of the int array.
    Due to the speed of int operations and the upper limit of N<=10^5.
    Doing a consolidation after a few operations or the O(NM) algorithm is faster than the O(NlogM) when consolidation is done after every M/logM operations.

*In addition to algorithms*
The key to reaching the speed was cache locality. Dynamically allocated nodes are too spread out for efficient operations.
By allocating an array of nodes larger than the maximum amount used, I can get very high cache hit rates



https://threads-iiith.quora.com/Treaps-One-Tree-to-Rule-em-all-Part-1
*/

struct node {
    int ind;
    int len;
    struct node * prev, * next;
    node () : ind(0),len(0), prev(NULL), next(NULL){};
    node (int i, int l,node * p, node * n) : ind(i), len(l), prev(p), next(n) {};
    void set (int i, int l,node * p, node * n) { ind=i; len=l; prev=p; next=n; };
};

static int nodeCount =0;
static node * head;
static node * tail;

void print (int * A);
void moveNodes (node * startNode, node * endNode, int type);
void recombine (int * &A, int N ){  // recombine the nodes, and restructure the array, don't forget to delete the dynamic memory
    int * tempA = A;
    int count = 1;
    node * cur = head;
    A = new int [N+1];
    A[0]=0;
    while (cur!=NULL){
        for (int i1 = 0; i1<cur->len ;i1++){
            A[count]= tempA[cur->ind+i1];
            count++;
        }
        cur = cur->next;
        //free (cur);
        //cur = next;
    }
    free (tempA);
}
void operate ( node * nodeArray, int type, int i, int j){
    node * temp, * cur=head, * startNode=NULL, * endNode=NULL;
    int s=1, e, tempLen;
    while (1){
        e=s+cur->len-1;
        if (e < i){ //H, still needs to keep moving
            s+=cur->len;
            cur=cur->next;
        } else if (s<i && e <= j) { // F, touch end only, i is in this block
            tempLen =cur->len;
            temp = cur->next;
            cur->len = i-s;
            //cur->next = new node (cur->ind+cur->len,tempLen-cur->len,cur,temp);
            cur->next = nodeArray+nodeCount;
            cur->next->set(cur->ind+cur->len,tempLen-cur->len,cur,temp);
            ++nodeCount;

            if (temp!=NULL)
                temp->prev=cur->next;

            startNode=cur->next;
            if (e==j){
                endNode = cur->next;
                break;
            }
            s+=tempLen;
            cur=temp;
        } else if (i <= s && e <= j ) { // touch both sides
            if (s==i)
                startNode = cur;
            if (e==j){
                endNode = cur;
                break;
            }
            s+=cur->len;
            cur=cur->next;
        } else if ( i <= s && s <= j && j < e){ //A, touch start only, j ends here
            tempLen =cur->len;
            temp = cur->next;
            cur->len = j-s+1;
            //cur->next = new node (cur->ind+cur->len,tempLen-cur->len,cur,temp);
            cur->next = nodeArray+nodeCount;
            cur->next->set(cur->ind+cur->len,tempLen-cur->len,cur,temp);
            ++nodeCount;

            if (temp!=NULL)
                temp->prev=cur->next;

            if (s==i)
                startNode = cur;
            endNode = cur;
            break;
        } else if ( s < i && j < e ) { //inside, but not touching either
            tempLen =cur->len;
            temp = cur->next;
            cur->len = i-s;
            node * curNext;
            //cur->next= new node (cur->ind+cur->len,j-i+1,cur,NULL);
            //substitute to save some dereferencing
            curNext = nodeArray+nodeCount;
            curNext->set(cur->ind+cur->len,j-i+1,cur,NULL);
            ++nodeCount;
            //curNext->next = new node (curNext->ind+curNext->len,e-j,curNext,temp);
            curNext->next = nodeArray+nodeCount;
            curNext->next->set(curNext->ind+curNext->len,e-j,curNext,temp);
            ++nodeCount;
            cur->next=curNext;

            if (temp!=NULL)
                temp->prev=cur->next->next;

            startNode = cur->next;
            endNode = cur->next;
            break;
        }
        else if (s > j){ //G
            cout<< "wrong";
            break;
        } else {
            cout<< "wrong";
        }
    }
    if (startNode==NULL || endNode==NULL){
        cout<< "wrong";
    }
    //print (head,tail,A);
    moveNodes (startNode,endNode,type);

    // move the nodes to front or end
}

void moveNodes (node * startNode, node * endNode, int type){
    while (tail->next != NULL)
        tail = tail->next;
    if (type == 1){
        if (head == startNode)
            return;
        if (tail == endNode)
            tail = startNode->prev;
        startNode->prev->next = endNode->next;

        if (endNode->next != NULL)
            endNode->next->prev=startNode->prev;
        head->prev = endNode;
        endNode->next=head;
        head = startNode;
        startNode->prev=NULL;
    }else {//move to back
        if (tail == endNode){
            return;
        }
        if (head == startNode)
            head = endNode->next;

        endNode->next->prev=startNode->prev;
        if (startNode->prev != NULL)
            startNode->prev->next = endNode->next;
        tail->next = startNode;
        startNode->prev=tail;
        tail = endNode;
        endNode->next=NULL;
    }
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N,M,type,i,j;
    cin >> N >> M;
    const int logM = (int) (log(M) / log(2));
    node * nodeArray = new node [logM*3];
    int * A = new int [N+1];
    A[0]=-1;
    for (int x =1 ; x<N+1;++x)
        cin >> A[x];
    //head = new node (1,N,NULL,NULL);
    head = nodeArray+nodeCount;
    head->set(1,N,NULL,NULL);nodeCount++;
    tail = head;
    for (int x=0, lc=0; x<M ; ++x,lc++){
        cin >> type >> i >> j;
        if (lc ==logM){ // recombine M/logM times, almost constant time
            recombine (A,N);
            nodeCount=0;
            head = nodeArray+nodeCount;
            head->set(1,N,NULL,NULL);
            ++nodeCount;
            tail = head;
            lc=0;
        }
        operate (nodeArray,type,i,j);
        //print (A);
    }
    recombine (A,N);
    cout<< abs(A[1]-A[N]) << '\n';
    for (int x=1;x<N+1;x++)
        cout << A[x] << ' ';
    return 0;
}

void print (int * A){
    node * cur = tail;
    while (cur->prev != NULL)
        cur=cur->prev;
    //cur should be at head
    cout << "head index value: "<<A[head->ind] << " tail index value: " << A[tail->ind]<<"\n";
    while (cur!=NULL){
        for (int i1 = 0; i1<cur->len ;i1++){
            cout << A[cur->ind+i1] << " ";
        }
        cout << ": ";
        cur=cur->next;
    }
    cout <<"\n";

    cur = head;
    while (cur!=NULL){
        for (int i1 = 0; i1<cur->len ;i1++){
            cout << A[cur->ind+i1] << " ";
        }
        cout << ": ";
        cur=cur->next;
    }
    cout <<"\n";
}
