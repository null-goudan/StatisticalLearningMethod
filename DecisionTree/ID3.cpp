#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <math.h>
#include <map>

using namespace std;
/* 
    algorithm : ID3 build DecisionTree
    input:  train dataset D     the therhold of feature Set: earth
    output: DecisionTree T
 */

class Data{
    public:
        vector<int> X;
        int label;
};

class Attr{
    public:
        double threhold;
        int feature_index;
        Attr()=default;
        Attr(int feature_index, double threhold): feature_index(feature_index), threhold(threhold){};
        bool operator<(const Attr other) const
        {
            return this->feature_index < other.feature_index;
        }
};

class ID3_Tree_Node{
    public:
        Attr data;
        ID3_Tree_Node* left;
        ID3_Tree_Node* right;
        int class_res = -1;
        ID3_Tree_Node(int feature_index, double threhold, ID3_Tree_Node* left, ID3_Tree_Node* right): data(Attr(feature_index,threhold)),
                                                                                            left(left), right(right){
                                                                                                cout<<"New tree node 1"<<endl;
                                                                                            };
        ID3_Tree_Node(Attr data, ID3_Tree_Node* left, ID3_Tree_Node* right): data(data), left(left),right(right){
            cout<<"New tree node 2"<<endl;
        };
        ID3_Tree_Node(int class_res):class_res(class_res),left(nullptr),right(nullptr){
            this->data.feature_index = -1;
            this->data.threhold = -1;
            cout<<"New tree node 3"<<endl;
        };
};

class ID3_Tree{
    public:
        ID3_Tree() = default;
        ID3_Tree(vector<Data> dataset, set<Attr> A);
        void printTree(ID3_Tree_Node* pnode);
        ID3_Tree_Node* root;
    private:
        
        multiset<int> m_X;
        multiset<int> m_Y;
        set<int> X_set;
        set<int> Y_set;
        map<int, double> infogains;
        vector<Data> dataset;
        int max_class;
        int best_feature_index;
        ID3_Tree_Node* build_Tree(vector<Data> D, set<Attr> A);
        double get_InformationGain(vector<Data> D, int feature_A);
        void get_allInfoGains(vector<Data> D, set<Attr> feature_set);
        int get_bestFeature();
        void split_Dataset(vector<Data>& out, int feature_index);
        void update_maxclass(vector<Data> D);
        void statistic(vector<Data> D, set<Attr> feature_set);
};

ID3_Tree::ID3_Tree(vector<Data> Dataset, set<Attr> earth){
    for(auto i:Dataset){
        this->dataset.push_back(i);
    }
    cout<<"Tree's Dataset loaded"<<endl;
    root = this->build_Tree(Dataset, earth);
}

// :TODO
ID3_Tree_Node* ID3_Tree::build_Tree(vector<Data> D, set<Attr> A){
    printf("building start....\n");
    if(A.empty() || m_Y.size() == 1){
        cout<<"A empty or m_Y size is one"<<endl;
        ID3_Tree_Node* res = new ID3_Tree_Node(this->max_class);
        return res;
    }
    cout<<"Not  A empty or m_Y size is one"<<endl;
    get_allInfoGains(D, A);
    cout<<"get all infomation gains successful"<<endl;
    pair<int, double> Ag;
    Ag.first = -1;
    Ag.second = 0.0;
    for(auto i : infogains){
        if(i.second > Ag.second){
            Ag.first = i.first;
            Ag.second = i.second;
        }
    }
    cout<<"get the Ag successfully"<<endl;
    // (4) look for the earth of Ag
    Attr earth;
    for(auto i:A){
        if(i.feature_index == Ag.first){
            earth.feature_index = i.feature_index;
            earth.threhold = i.threhold;
            break;
        }
    }
    cout<<"looked for the earth successful"<<endl;
    if(earth.threhold > Ag.second){
        cout<<"the threhold is bigger than Ag's infomation gains"<<endl;
        ID3_Tree_Node* aa = new ID3_Tree_Node(this->max_class);
        ID3_Tree_Node* res = aa;
        return res;
    }
    // (5)
    split_Dataset(D, Ag.first);
    get_allInfoGains(D, A);
    statistic(D, A);
    ID3_Tree_Node* res = new ID3_Tree_Node(earth, nullptr, nullptr);
    res->left = new ID3_Tree_Node(max_class);
    // (6)
    set<Attr>::iterator ag;
    for(ag = A.begin(); ag!=A.end(); ag++){
        if(ag->feature_index==earth.feature_index&&ag->threhold==earth.threhold){
            break;
        }
    }
    A.erase(ag);
    res->right = build_Tree(D, A);
    return res;
}

double ID3_Tree::get_InformationGain(vector<Data> D, int feature_A){
    // (1) calc the empirical entorpy of dataset D :  H(D)
    m_Y.clear();
    Y_set.clear();
    m_X.clear();
    X_set.clear();
    map<int, map<int, int>> m;
    
    double H_D = 0.0;
    double H_D_A = 0.0;
    for(auto i: D){
        Y_set.insert(i.label);
        m_Y.insert(i.label);
        m_X.insert(i.X[feature_A]);
        X_set.insert(i.X[feature_A]);
        m[i.X[feature_A]][i.label] ++;
    }
    for(auto c: Y_set){
        H_D -= (m_Y.count(c)*1.0f/m_Y.size()) * log2f64(m_Y.count(c)*1.0f/m_Y.size()) ;
    }
    for(auto i: X_set){
        double B_sum = 0.0;
        for(auto c: Y_set){
            if(m[i][c]){
                B_sum += (m[i][c]*1.0f/m_X.count(i))*log2f64(m[i][c] * 1.0f / m_X.count(i));
            }
        }
        H_D_A -= (m_X.count(i)* 1.0f  / m_X.size() * 1.0f )*(B_sum);
    }
    return double(H_D-H_D_A);
}

//get the max class
void ID3_Tree::update_maxclass(vector<Data> D){
    int max_count = 0;
    for(auto c:Y_set){
        if(max_count < m_Y.count(c)){
            max_class = c;
            max_count = m_Y.count(c);
        }
    }
}

void ID3_Tree::statistic(vector<Data> D, set<Attr> feature_set){
    max_class = -1;
    this->best_feature_index = get_bestFeature();
    update_maxclass(D);
    cout<<"max_class\t"<<max_class<<endl;
}

void ID3_Tree::get_allInfoGains(vector<Data> D, set<Attr> feature_set){
    infogains.clear();
    for(auto i:feature_set){
        infogains[i.feature_index] = get_InformationGain(D, i.feature_index);
        printf("infogains[%d] = %lf\n", i.feature_index, infogains[i.feature_index]);
    }
}

int ID3_Tree::get_bestFeature(){
    int bestFeature_index;
    double maxinfogain = 0.0f;
    for(auto i:infogains){
        if(i.second>maxinfogain){
            bestFeature_index = i.first;
            best_feature_index = i.first;
            maxinfogain = i.second;
            break;
        }
    }
    return bestFeature_index;
}

void ID3_Tree::split_Dataset(vector<Data>& out, int feature_index){
    int cnt = 0;
    for(auto i: out){
        if(i.label == this->max_class){
            out.erase(out.begin()+cnt);
        }
        ++cnt;
    }
}

// read dataset file
void readDataset(vector<Data>& Dataset, int feature_num, string filepath){
    Dataset.clear();
    ifstream in;
    in.open(filepath);
    if(in){
        string curline;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            Data per;
            int i = 0;
            while(getline(ss, temp, ' ')){
                if(i == feature_num){
                    per.label = stoi(temp);
                }else{
                    per.X.push_back(stoi(temp));
                }
                i++;
            }
            Dataset.push_back(per);
        }
    }else{
        cout<<"Don't read the file to train Dataset"<<endl;
    }
}

void ID3_Tree::printTree(ID3_Tree_Node* pnode){
    if(pnode==nullptr){
        return ;
    }
    printTree(pnode->left);
    if(pnode->class_res!=-1){
        cout<<pnode->class_res<<"\t";
    }else{
        cout<<"("<<pnode->data.feature_index<<","<<pnode->data.threhold<<")\t";
    }
    printTree(pnode->right);
}


int main(){
    vector<Data> traindataset;
    readDataset(traindataset, 4, "./train.txt");
    cout<<"trian set loaded"<<endl;
    set<Attr> attrs;
    attrs.insert(Attr(1, 0.2));
    attrs.insert(Attr(2, 0.3));
    cout<<"attrs set loaded"<<endl;
    ID3_Tree tree(traindataset, attrs);
    tree.printTree(tree.root);
    return 0;
}