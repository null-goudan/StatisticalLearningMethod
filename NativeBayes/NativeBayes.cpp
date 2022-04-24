#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

using namespace std;

class Data{
    public:
        Data() =default;
        Data(vector<double> X, int label);
        vector<double> X;
        int label;
};

Data::Data(vector<double> X, int label){
    this->X.clear();
    for(auto i : X){
        this->X.push_back(i);
    }
    this->label = label;
}

class P_Xi_yi{
    public:
        double xi;
        double yi;
        double P;
        int index;
};

class NativeBayes{
    public:
        NativeBayes() = default;
        NativeBayes(vector<Data> train_dataset);
        void loadTrainDataset(vector<Data> train_dataset);
        int predict(Data& pdata);
    private:
        vector<Data> train_dataset;
        vector<multiset<double>> m_X;
        multiset<int> m_Y;
        vector<set<double>> set_X;
        set<int> set_Y;
        map<int, double> P_Y;
        vector<P_Xi_yi> P_xi_yi;

        void calcP();
};

NativeBayes::NativeBayes(vector<Data> train_dataset){
    this->train_dataset.clear();
    this->m_X.clear();
    this->m_Y.clear();
    this->set_X.clear();
    this->set_Y.clear();
    this->P_Y.clear();
    this->P_xi_yi.clear();
    for(auto i: train_dataset){
        this->train_dataset.push_back(i);
    }
}

void NativeBayes::loadTrainDataset(vector<Data> train_dataset){
    this->train_dataset.clear();
    this->m_X.clear();
    this->m_Y.clear();
    this->set_X.clear();
    this->set_Y.clear();
    this->P_Y.clear();
    this->P_xi_yi.clear();
    for(auto i: train_dataset){
        this->train_dataset.push_back(i);
    }
}

void NativeBayes::calcP(){
    if(this->train_dataset.empty()){
        this->P_xi_yi.clear();
        this->P_Y.clear();
        return ;
    }
    for(auto i: train_dataset){
        this->m_Y.insert(i.label);
        this->set_Y.insert(i.label);
    }
    for(int i = 0;i<train_dataset[0].X.size();i++){
        multiset<double> xi;
        set<double> xi_set;
        for(Data k : train_dataset){
            // printf("k.X[i]:%f\n", k.X[i]);
            xi.insert(k.X[i]);
            xi_set.insert(k.X[i]);
        }
        this->m_X.push_back(xi);
        this->set_X.push_back(xi_set);
    }
    double N = double(train_dataset.size());

    for(auto i: set_Y){
        P_Y[i] =  m_Y.count(i)/N;
        cout<<"P("<<i<<")="<<P_Y[i]<<endl;
    }
    int index = 0;
    for(set<double> i: set_X){
        for(auto j: set_Y){
            for (double xi: i){
                P_Xi_yi x;
                // printf("xi :%f\n", xi);
                x.xi = xi;
                x.yi = j;
                x.index = index;
                int count_xi_y = 0;
                for(int k=0;k<train_dataset.size();k++){
                    if(train_dataset[k].X[index]==x.xi&&train_dataset[k].label == x.yi){
                    // cout<<"train_dataset["<<k<<"].X["<<index<<"] = "<<train_dataset[k].X[index];
                    // cout<<"\t x.xi = "<<x.xi<<endl;
                    // cout<<"train_dataset["<<k<<"].label = "<<train_dataset[k].label;
                    // cout<<"\t x.yi = "<<x.yi<<endl;
                        ++count_xi_y;
                    }
                }
                // cout<<count_xi_y<<endl;
                x.P = count_xi_y / (double) m_Y.count(x.yi);
                this->P_xi_yi.push_back(x);
            }
        }
        ++index;
    }
    cout<<"-------------------"<<endl;
    for(auto i: this->P_xi_yi){
        cout<<"P(x"<<i.index<<"="<<i.xi<<"|yi="<<i.yi<<")="<<i.P<<endl;
    }
    cout<<"--------------------"<<endl;
}

int NativeBayes::predict(Data& pdata){
    this->calcP();
    const int plabel = pdata.label;
    cout<<plabel<<endl;
    double Pmax = 0.0;
    double Pnow = 1;
    double nowmaxclass = 0;
    cout<<"result----------"<<endl;
    for(auto c: this->set_Y){
        Pnow = 1.0;
        int index = 0;
        for(auto i:pdata.X){
            for(auto j:this->P_xi_yi){
                // printf("plabel: %f, j.yi: %f\n", double(plabel), j.yi);
                if(i == j.xi && double(plabel)==j.yi&&j.yi==c&&index==j.index){
                    // printf("i:%d plabel: %f, j.yi: %f\n",i, double(plabel), j.yi);
                    Pnow*=j.P;
                    cout<<"P(x"<<j.index<<"="<<j.xi<<"|yi="<<j.yi<<")="<<j.P<<endl;
                }
            }
            ++index;
        }
        Pnow *= this->P_Y[c];
        // cout<<this->P_Y[c]<<endl;
        cout<<"P(Y="<<c<<")="<<Pnow<<endl;
        if(Pnow<=Pmax){
            Pmax=Pnow;
            nowmaxclass=c;
        }
    }
    return nowmaxclass;
}

void readtrainset(vector<Data>& train_dataset, string filepath, int feature_num){
    ifstream in;
    in.open(filepath);
    if(in){
        // read feature
        string curline;
        int i = 0;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            Data per;
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
            train_dataset.push_back(per);
        }
    }else{
        cout<<"Read trainDataset failed!: don't open the file"<<endl;
        return ;
    }
}

int main(){
    vector<Data> train_dataset;
    vector<double> X;
    X.push_back(1);
    X.push_back(1);
    readtrainset(train_dataset, "./train.txt", 2);
    NativeBayes a(train_dataset);
    cout<<"init traindataset"<<endl;
    Data x(X, 1);
    a.predict(x);
    return 0;
}