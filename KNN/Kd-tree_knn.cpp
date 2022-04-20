#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
using namespace std;

typedef struct kd_data{
    vector<double> X;
    int label;
}KD_DATA;
/* water function
    my data is 5 dim
*/

/*
 kd-tree node:
    node_data: k dim data node
    split-dim: split dim num
    left: left-child
    right: left-right 
*/
class kd_tree_node{
    public:
        KD_DATA data;
        kd_tree_node* left;
        kd_tree_node* right;
        kd_tree_node* parent;

        kd_tree_node(){
            this->left = nullptr;
            this->right = nullptr;
            this->parent = nullptr;
        }

        kd_tree_node(kd_data data){
            this->data = data;
            this->left = nullptr;
            this->right = nullptr;
            this->parent = nullptr;
        }
};


class kd_tree{
    public:
        kd_tree()=default;
        kd_tree(vector<KD_DATA> train_dataSet);

        void printKD_tree();
    private:
        kd_tree_node* root;
        int size;
        kd_tree_node* BuildKDTree(const std::vector<KD_DATA>& point_cloud, int split_dim = 0);
};  
/*
algorithm:
 build kd-tree:
 (1) input: k dim vector dataset : T = {x1 ,x2 , ... xN}
        xi = {x(1)i, x(2)i,....x(k)i};
     output: kd-tree
*/
kd_tree::kd_tree(vector<KD_DATA> train_dataSet){
    this->size = train_dataSet.size();
	this->root = BuildKDTree(train_dataSet);
}

kd_tree_node* kd_tree::BuildKDTree(const std::vector<KD_DATA>& point_cloud, int split_dim = 0){
    
    if(point_cloud.size() == 0){
        return nullptr;
    }
    sort(point_cloud.begin(), point_cloud.end(), [this, split_dim](KD_DATA a,KD_DATA b){
        return a.X[split_dim] < b.X[split_dim];
    });
    int mid_idx = point_cloud.size() / 2;
    kd_tree_node* root = new kd_tree_node(point_cloud[mid_idx]);
    // split
    vector<KD_DATA> left_cloud;
    vector<KD_DATA> right_cloud;
    left_cloud.insert(left_cloud.begin(),point_cloud.begin(), point_cloud.begin()+mid_idx);
    right_cloud.insert(right_cloud.begin(), point_cloud.begin()+mid_idx+1, point_cloud.end());

    split_dim = (split_dim % point_cloud[0].X.size()) + 1;

    //build right and left
    root->left = BuildKDTree(left_cloud, split_dim);
    root->right = BuildKDTree(right_cloud, split_dim);

    //设置父节点
	if (root->left != nullptr)
	{
		root->left->parent = root;
	}
	
	if (root->right != nullptr)
	{
		root->right->parent = root;
	}

    return root;
}

void kd_tree::printKD_tree(){
    
}