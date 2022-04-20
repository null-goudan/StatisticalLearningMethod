#include <iostream>

#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <map>

using namespace std;
/*
    Linear KNN algorthm
    input:   train Data_Mset: T= {(x1, y1), (x2,y2) ... (xN, yN)}
                                xi belong to R  yi = {c1, c2, .... cK} k classes
    output:  the classes x belong to

    algorthm step:
    (1): According to the given distance measure, find the k nearest points of X in the training set t,
     and the neighborhood of X covering these k points is recorded as Nk(x)
        tip : distance: Eclidean distance
    (2): Determine the category of X according to the classification decision rules (such as majority voting)
        tip : this program use majority voting
*/

const int label_num = 4;

class Data_M{
    public:
        vector<double> X;
        int label;
        double dis;
        int result;
        void get_dis(Data_M& x);
};

// Eclidean distance
void Data_M::get_dis(Data_M& x){
    int len = this->X.size();
    for(int i = 0;i<len;i++){
        this->dis += (x.X[i] - this->X[i]) * (x.X[i] - this->X[i]);
    }
    this->dis = sqrt(this->dis);
}

class Knn_manager{
    public:
        Knn_manager()=default;
        Knn_manager(int k);
        string knn(Data_M& x);
        void input_trainData_Mset(int feature_num, const string filepath);
        void input_testData_Mset(int feature_num, const string filepath);
    private:
        vector<Data_M> train_dataSet;
        vector<Data_M> test_Data_Mset;
        int k;
        void get_all_dis(Data_M& x);
        const string label_name[4] = {"very low", "low", "middle", "high"};
};

bool cmp(Data_M& a, Data_M& b){
    return a.dis < b.dis;
}

Knn_manager::Knn_manager(int k):k(k){
    this->train_dataSet.clear();
    this->test_Data_Mset.clear();
}

void Knn_manager::input_trainData_Mset(int feature_num, const string filepath){
    ifstream in;
    in.open(filepath);
    if(in){
        // read feature
        string curline;
        int i = 0;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            Data_M per;
            int i = 0;
            while(getline(ss, temp, ' ')){
                if(i  == feature_num){
                    per.label = stod(temp);
                }
                else{
                    per.X.push_back(stod(temp));
                }
                i++;
            }
            train_dataSet.push_back(per);
        }
    }else{
        cout<<"Read trainData_Mset failed!: don't open the file"<<endl;
        return ;
    }
}

void Knn_manager::input_testData_Mset(int feature_num, const string filepath){
    fstream in;
    in.open(filepath);
    if(in){
        // read feature
        string curline;
        int i = 0;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            Data_M per;
            int i = 0;
            while(getline(ss, temp, ' ')){
                if(i  == feature_num){
                    per.label = stod(temp);
                }
                else{
                    per.X.push_back(stod(temp));
                }
                i++;
            }
            test_Data_Mset.push_back(per);
        }
    }else{
        cout<<"Read testData_Mset failed!: don't open the file"<<endl;
        return ;
    }
}

void Knn_manager::get_all_dis(Data_M& x){
    int Data_Mset_size = this->train_dataSet.size();
    int feature_size = this->train_dataSet[0].X.size();
    for(int i =0;i<Data_Mset_size; i++){
        this->train_dataSet[i].get_dis(x);
    }
}

string Knn_manager::knn(Data_M& x){
    this->get_all_dis(x);
    sort(this->train_dataSet.begin(), this->train_dataSet.end(), cmp);
    map<int, int> m;
    for(int i=0;i<label_num;i++){
        m[i] = 0;
    }
    for(int i =0;i<k;i++){
        cout<<"max i th :"<<train_dataSet[i].dis<<endl;
        cout<<"feature :";
        for(int j=0;j<4;j++){
            cout<<train_dataSet[i].X[j]<<" ";
        }
        cout<<"label: "<<train_dataSet[i].label;
        cout<<endl;
        m[train_dataSet[i].label]++;
    }
    double max = -1;
    int index = -1;
    for(auto i : m){
        if(i.second > max){
            max = i.second;
            index = i.first;
        }
    }
    cout<<"max: \t"<<max<<endl;
    cout<<"index: \t"<<index<<endl;
    return this->label_name[index];
}

int main(){
    int k ;
    cout<<"input k"<<endl;
    cin>>k;
    Knn_manager knn(k);
    knn.input_trainData_Mset(4, "./train.txt");
    Data_M x;
    for(int i =0;i<4;i++){
        double da;
        cin>>da;
        x.X.push_back(da);
    }
    string res = knn.knn(x);
    cout<<"res: \t"<<res<<endl;
}