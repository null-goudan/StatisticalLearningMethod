#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
using namespace std;

typedef class kd_data{
public:
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
        kd_tree_node* BuildKDTree(std::vector<KD_DATA>& point_cloud, int split_dim);
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
	this->root = BuildKDTree(train_dataSet, 0);
}

kd_tree_node* kd_tree::BuildKDTree(std::vector<KD_DATA>& point_cloud, int split_dim){
    if(point_cloud.size() == 0){
        return nullptr;
    }
    sort(point_cloud.begin(), point_cloud.end(), [split_dim](KD_DATA a,KD_DATA b){
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
    queue<pair<kd_tree_node*, int>> nodes;
    nodes.push(pair<kd_tree_node*, int>(this->root, 0));
    int last_layer = 0;
    while(!nodes.empty()){
        auto curnode = nodes.front();
        if(curnode.first!=nullptr){
            if(curnode.second > last_layer){
                last_layer ++;
                cout<<endl;
            }
            if(curnode.second<last_layer){
                cout<<"(";
                for(auto item : curnode.first->data.X){
                    cout<<item<<",";
                }
                cout<<") ";
            }else{
                cout<<"(";
                for(auto item : curnode.first->data.X){
                    cout<<item<<",";
                }
                cout<<") ";
            }
            nodes.push(pair<kd_tree_node*, int>(curnode.first->left, curnode.second+1));
            nodes.push(pair<kd_tree_node*, int>(curnode.first->right, curnode.second));
        }
        nodes.pop();
    }
}

int main(){
    vector<KD_DATA> trainset;
    KD_DATA data1;
    data1.X.push_back(2);
    data1.X.push_back(3);
    trainset.push_back(data1);
    data1.X.clear();
    data1.X.push_back(5);
    data1.X.push_back(4);
    trainset.push_back(data1);
    data1.X.clear();
    data1.X.push_back(9);
    data1.X.push_back(6);
    trainset.push_back(data1);
    data1.X.clear();
    data1.X.push_back(4);
    data1.X.push_back(7);
    trainset.push_back(data1);
    data1.X.clear();
    data1.X.push_back(8);
    data1.X.push_back(1);
    trainset.push_back(data1);
    data1.X.clear();
    data1.X.push_back(7);
    data1.X.push_back(2);
    trainset.push_back(data1);
    for(auto i : trainset){
        cout<<i.X[0]<<"\t";
    }
    cout<<endl;
    cout<<"train dataset is finished"<<endl;
    kd_tree k(trainset);
    k.printKD_tree();
    return 0;
}