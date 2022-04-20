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
        int split_dim;
        kd_tree_node* left;
        kd_tree_node* right;
        kd_tree_node* parent;

        kd_tree_node(){
            this->split_dim = 0;
            this->left = nullptr;
            this->right = nullptr;
            this->parent = nullptr;
        }

        kd_tree_node(kd_data& data){
            this->data = data;
            this->split_dim = 0;
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
        int maxv_pos;

        kd_tree_node* BuildKDTree(const std::vector<KD_DATA>& point_cloud);
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

kd_tree_node* kd_tree::BuildKDTree(const std::vector<KD_DATA>& point_cloud){
    
}