#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

double sigmoid(double x);
double scalarProduct(vector<double> w, vector<double> x, double bias = 0);
void GradAscent(vector<vector<double>> DataSet_Feature, vector<double> Label, vector<double>& weight, double alpha);
vector<vector<double>> MatTransponse(vector<vector<double>> Mat);

class Logistic{

    public:
        Logistic()=default;
        Logistic(int feature_num, string filename){
            loadDataset(feature_num, filename);
            this->weight = vector<double>(feature_num);
        }
        void train(int iterNum, double lr = 0.003, void(*)(vector<vector<double>>, vector<double>, vector<double>&, double)=GradAscent);
        void loadDataset(int feature_num, string filename);
        inline int pridict(vector<double> feature);
        inline vector<double> get_labels();
        inline vector<vector<double>> get_featuresMat();
        inline vector<double> get_weight();
    private:
        vector<vector<double>> feature_Mat;
        vector<double> label_Mat;
        vector<double> weight;
};

vector<vector<double>> MatTransponse(vector<vector<double>> Mat){
    int size_h = Mat.size();
    int size_w = Mat[0].size();
    vector<vector<double>> res(size_w, vector<double>(size_h));
    for(int i =0;i<size_h ;++i){
        for(int j =0;j<size_w;++j){
            res[j][i] = Mat[i][j];
        }
    }
    return res;
}

void Logistic::loadDataset(int feature_num, string filename){
    ifstream in;
    in.open(filename, ios::in);
    string line;
    while(getline(in,line)){
        vector<double> x;
        double y;
        stringstream ss(line);
        string temp;
        int i = 0;
        while(getline(ss, temp, ' ')){
            if(++i % (feature_num + 1) == 0){
                y = stod(temp);
                // cout<< y<<endl;
            }else{
                x.push_back(stod(temp));
                // cout<<stod(temp)<<" ";
            }
        }
        this->feature_Mat.push_back(x);
        this->label_Mat.push_back(y);
    }
}

inline vector<double> Logistic::get_labels(){
    return this->label_Mat;
}

inline vector<vector<double>> Logistic::get_featuresMat(){
    return this->feature_Mat;
}

inline vector<double> Logistic::get_weight(){
    return this->weight;
}

void Logistic::train(int iterNum, double lr, void(* train_method)(vector<vector<double>>, vector<double>, vector<double>&, double)){
    if(iterNum == 0) return ;
    if(this->feature_Mat.size()==0){printf("error! DataSet not load, Don't train!\n"); return ;}
    if(this->weight.size()==0){printf("Weight error! Don't train!\n"); return ;}
    while(iterNum--){
        train_method(this->feature_Mat, this->label_Mat, this->weight, lr);
    }
}

inline int Logistic::pridict(vector<double> feature){
    const int size = this->weight.size();
    double res = 0.0;
    for(int i =0;i<size;i++){
        res +=  feature[i] * this->weight[i];
    }
    cout<< res<<" ";
    res = sigmoid(res);
    cout<<res<< " " ;
    if(res>=0.5){
        return 1;
    }
    else{
        return 0;
    }
}

double sigmoid(double x){
    return exp(x) / (1 + exp(x));
}

double scalarProduct(vector<double> w, vector<double> x, double bias){
    double ret=0;
    for(int i=0;i<w.size();i++)
        ret+=w[i]*x[i];
    ret += bias;
    return ret;
}

void GradAscent(vector<vector<double>> DataSet_Feature, vector<double> Label, vector<double>& weight, double alpha){
    vector<vector<double>> dataMatT=MatTransponse(DataSet_Feature);
    vector<double> h;
    vector<double> error;
    for(auto &data:DataSet_Feature)
        h.push_back(sigmoid(scalarProduct(data,weight)));
    for(int i=0;i<Label.size();i++)
        error.push_back(Label[i]-h[i]);
    for(int i=0;i<weight.size();i++){
        weight[i] +=alpha*scalarProduct(dataMatT[i],error);
    }
}

vector<vector<double>> random_datatest(){
    vector<vector<double>> res;
    default_random_engine e;
    uniform_real_distribution<double> u_x1(0.0, 3.5);
    uniform_real_distribution<double> u_x2(13.0, 20.5);
    uniform_real_distribution<double> u_x1_2(0.0, 3.5);
    uniform_real_distribution<double> u_x2_2(13.0, 20.5);
    for(int i =0;i<30;i++){
        vector<double> resp;
        resp.push_back(u_x1(e));
        resp.push_back(u_x2(e));
        res.push_back(resp);
    }
    for(int i =0;i<30;i++){
        vector<double> resp;
        resp.push_back(u_x1_2(e));
        resp.push_back(u_x2_2(e));
        res.push_back(resp);
    }
    return res;
}

int main(){
    vector<vector<double>> Feature_set;
    vector<double> label;
    string filname("dataset.txt");
    Logistic L(2, filname);
    L.train(50000);
    vector<double> w;
    w = L.get_weight();
    for(auto i:w){
        cout<<i<<" ";
    }
    // vector<double> sss;
    // sss.push_back(5.2);
    // sss.push_back(11.2);
    // cout<<L.pridict(sss)<<endl;
    vector<vector<double>> Feature_test;
    ofstream f;
    f.open("out.txt", ios::out);
    Feature_test = random_datatest();
    for(int i =0;i<Feature_test.size();++i){
        for(auto j:Feature_test[i]){
            f<<j<<" ";
        }
        f<<L.pridict(Feature_test[i])<<endl;
    }
    ofstream f1;
    f1.open("weight.txt", ios::out);
    for(auto i:w){
        f1<<i<<" ";
    }
    return 0;
}