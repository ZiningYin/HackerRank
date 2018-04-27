//https://www.hackerrank.com/challenges/self-balancing-tree/problem
/*
typedef struct node
{
    int val;
    struct node* left;
    struct node* right;
    int ht;
} node; */
int getBalance (node *root);
int getHeight (node *root);
node * balanceRight (node *root);
node * balanceLeft (node *root);

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
    else {
        root->left=insert(root->left,val);
        if (getBalance(root)>1)
            root=balanceLeft(root);
        else
            root->ht=getHeight(root);   
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
