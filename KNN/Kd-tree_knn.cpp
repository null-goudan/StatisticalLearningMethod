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

/*
 kd-tree node:
    node_data: k dim data node
    split-dim: split dim num
    left: left-child
    right: left-right 
    is-leaf: check whether a leaf node
*/
class kd_tree_node{
    public:
        KD_DATA data;
        kd_tree_node* left;
        kd_tree_node* right;
        kd_tree_node* parent;
        
        int split_dim;

        bool is_leaf;

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
        bool findKNearestNodeByR(vector<KD_DATA>& point_cloud, KD_DATA& xpoint,vector<KD_DATA>& npoints_output, double redius, int k);
    private:
        kd_tree_node* root;
        int size;
        kd_tree_node* BuildKDTree(std::vector<KD_DATA>& point_cloud, int split_dim);
        kd_tree_node* findLeafNode(kd_tree_node* pnode,KD_DATA& pos_point);
        void findNPointInRNode(kd_tree_node* pnode,KD_DATA& xpoint,vector<KD_DATA>&npoint_output, double redius);
        void findNPointInRNode_D(kd_tree_node* pnode,KD_DATA& xpoint,vector<KD_DATA>&npoint_output, double redius);
       
        double Dist(const KD_DATA& point1, const KD_DATA& point2);
};

double kd_tree::Dist(const KD_DATA& point1, const KD_DATA& point2){
    double sum = 0.0;
    int dim_all = point1.X.size();
    for(int i =0;i<dim_all;i++){
        sum += (point1.X[i] - point2.X[i])*(point1.X[i] - point2.X[i]);
    }
    return sqrt(sum);
}
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
    kd_tree_node* pnode = new kd_tree_node(point_cloud[mid_idx]);
    // split
    vector<KD_DATA> left_cloud;
    vector<KD_DATA> right_cloud;
    left_cloud.insert(left_cloud.begin(),point_cloud.begin(), point_cloud.begin()+mid_idx);
    right_cloud.insert(right_cloud.begin(), point_cloud.begin()+mid_idx+1, point_cloud.end());
    pnode->split_dim = split_dim;
    split_dim = ((split_dim + 1 ) % point_cloud[0].X.size());
    //build right and left
    pnode->left = BuildKDTree(left_cloud, split_dim);
    pnode->right = BuildKDTree(right_cloud, split_dim);

    //设置父节点
	if (pnode->left != nullptr)
	{
		pnode->left->parent = pnode;
	}
	
	if (pnode->right != nullptr)
	{
		pnode->right->parent = pnode;
	}

    if(pnode->left == nullptr && pnode->right == nullptr){
        pnode->is_leaf = true;
    }

    return pnode;
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
                cout<<"split  "<<curnode.first->split_dim<<":dim num: ("<<curnode.first->data.X[curnode.first->split_dim]<<")"<<") ";
                if(curnode.first->is_leaf){
                    cout<<"<-leaf   ";
                }
            }else{
                cout<<"(";
                for(auto item : curnode.first->data.X){
                    cout<<item<<",";
                }
                cout<<"split  "<<curnode.first->split_dim<<":dim num: ("<<curnode.first->data.X[curnode.first->split_dim]<<")"<<") ";
                if(curnode.first->is_leaf){
                    cout<<"<-leaf   ";
                }
            }
            nodes.push(pair<kd_tree_node*, int>(curnode.first->left, curnode.second+1));
            nodes.push(pair<kd_tree_node*, int>(curnode.first->right, curnode.second));
        }
        nodes.pop();
    }
    cout<<endl;
}

/*
algorithm:
 seach the nearest node with the input point in kd-tree 
 (1) find the leaf node, if the data num which split dim is less than pos_point choose left child
        the leaf node found will be as the nearest node
 (2) recursion to find the leadest node 
*/
kd_tree_node* kd_tree::findLeafNode(kd_tree_node* pnode,KD_DATA& pos_point){
    if(pnode==nullptr){
        return nullptr;
    }
        // KD_DATA data = pnode->data;
        // cout<<"(";
        // for(auto i : data.X){
        //     cout<<i<<",";
        // }
        // cout<<"split dim:"<<pnode->split_dim<<")"<<endl;;
    if(pnode->is_leaf){
        return pnode;
    }
    if(pnode->data.X[pnode->split_dim]>pos_point.X[pnode->split_dim]){
        return findLeafNode(pnode->left, pos_point);
    }else{
        auto ret = findLeafNode(pnode->right, pos_point);
        if(ret == nullptr && pnode->data.X[pnode->split_dim]==pos_point.X[pnode->split_dim]){
            ret = pnode;
        }
        return ret;
    }
    return nullptr;
}

void kd_tree::findNPointInRNode(kd_tree_node* pnode,KD_DATA& xpoint,vector<KD_DATA>&npoint_output, double redius){
    if(pnode->parent == nullptr){
        return ;
    }
    kd_tree_node* curnode = pnode->parent;
    if(Dist(curnode->data, xpoint)<=redius){
        npoint_output.push_back(pnode->data);
    }
    kd_tree_node* oth_node = curnode->left;
    if(oth_node == pnode){
        oth_node = curnode->right;
    }
    if(oth_node!=nullptr){
        double dist = fabs(xpoint.X[oth_node->split_dim] - oth_node->data.X[oth_node->split_dim]);
        if(dist<=redius){
            findNPointInRNode_D(oth_node, xpoint, npoint_output, redius);  // go to down
        }
    }
    findNPointInRNode(curnode, xpoint, npoint_output, redius);
}

void kd_tree::findNPointInRNode_D(kd_tree_node* pnode,KD_DATA& xpoint,vector<KD_DATA>&npoint_output, double redius){
    if (Dist(xpoint, pnode->data) <= redius)
	{
		npoint_output.push_back(pnode->data);
	}

	if (pnode->left != nullptr)
	{
		double dist = std::fabs(xpoint.X[pnode->split_dim] - pnode->left->data.X[pnode->split_dim]);
		if (dist <= redius)
		{
			findNPointInRNode_D(pnode->left, xpoint, npoint_output, redius);
		}
	}

	if (pnode->right != nullptr)
	{
		double dist = std::fabs(xpoint.X[pnode->split_dim] - pnode->left->data.X[pnode->split_dim]);
		if (dist <= redius)
		{
			findNPointInRNode_D(pnode->right,  xpoint, npoint_output, redius);
		}
	}
}

bool kd_tree::findKNearestNodeByR(vector<KD_DATA>& point_cloud, KD_DATA& xpoint,vector<KD_DATA>& npoints_output, double redius, int k){
    // printKD_tree();
    if(redius<=0){
        return false;
    }
    vector<KD_DATA> npoint;
    kd_tree_node* curnode = findLeafNode(root, xpoint);
    if(curnode == nullptr){
        cout<<"nullptr"<<endl;
        return true;
    }

    cout<<"nearest leaf node: \t(";
    std::cout << curnode->data.X[0] << "," << curnode->data.X[1] <<")"<< std::endl;

    if(Dist(curnode->data, xpoint)<=redius){
        npoint.push_back(curnode->data);
    }

    findNPointInRNode(curnode, xpoint, npoint, redius);

    if(k>0){
        sort(npoint.begin(), npoint.end(), [this, xpoint](KD_DATA& a, KD_DATA& b){
            return this->Dist(a, xpoint) < this->Dist(b, xpoint);
        });
        if (k > (int)(npoint.size()))
		{
			k = npoint.size();
		}
    }
    else{
        k = npoint.size();
    }
    npoints_output.resize(k);
    for(int i=0;i<k;i++){
        cout<<"(";
        for(auto j:npoint[i].X){
            cout<<j<<",";
        }
        cout<<")"<<endl;
        npoints_output.push_back(npoint[i]);
    }
    return true;
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
    cout<<"train dataset is finished"<<endl;
    kd_tree k(trainset);
    k.printKD_tree();
    // KD_DATA data1;
    data1.X.clear();
    data1.X.push_back(1);
    data1.X.push_back(1);
    vector<KD_DATA> k_near_point;
    k.findKNearestNodeByR(trainset, data1, k_near_point, 100, 1);
    // for(auto i:k_near_point){
    //     cout<<"(";
    //     for(auto j:i.X){
    //         cout<<j<<",";
    //     }
    //     cout<<")"<<endl;
    // }
    return 0;
}