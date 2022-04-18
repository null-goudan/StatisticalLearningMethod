#include <iostream>
#include "perceptron.h"
#include "model.h"

#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

// base perceptron algorithm: 

/* input:
    T:{(x1,y1), (x2,y2) .... (xN, yN)} 
    xi belong to R  yi belong to {-1, +1} 
    learning rate(n) : 0 < n <= 1
output:   w, b  perceptron model: f(x) = sign( w*x + b )
*/

/*
    step:
        (1): choose initial value: w0, b0
        (2): choose data(xi, yi) from train dataset
        (3): if yi(w*xi + b) <= 0 : 
                w  <--  w + n*yi*xi
                b  <--  b + n*yi
        (4): goto (2) util no error classify point
*/
# if 1
// normal 
void Read_trainDataset(vector<pair<vector<double>, double>>& train_dataset, const string filepath, int feature_num){
    ifstream in;
    in.open(filepath);
    if(in){
        // read feature
        string curline;
        int i = 0;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            pair<vector<double>, double> per;
            int i = 0;
            while(getline(ss, temp, ' ')){
                if(i  == feature_num){
                    per.second = stod(temp);
                }
                else{
                    per.first.push_back(stod(temp));
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

void Read_testDataset(vector<pair<vector<double>, double>>& test_dataset, const string filepath, int feature_num){
    ifstream in;
    in.open(filepath);
    if(in){
        // read feature
        string curline;
        int i = 0;
        while(getline(in, curline)){
            stringstream ss(curline);
            string temp;
            pair<vector<double>, double> per;
            int i = 0;
            while(getline(ss, temp, ' ')){
                if(i  == feature_num){
                    per.second = stod(temp);
                }
                else{
                    per.first.push_back(stod(temp));
                }
                i++;
            }
            test_dataset.push_back(per);
        }
    }else{
        cout<<"Read trainDataset failed!: don't open the file"<<endl;
        return ;
    }
}

void training(vector<pair<vector<double>, double>> train_dataset, int feature_num, vector<double>& w, double& bias, int lr, int step){
    for(int i = 0;i<step; i++){
        for(auto j : train_dataset){
            double loss = 0.0f;
            for(int k=0;k<feature_num;k++){
                loss += w[k] * j.first[k];
            }
            loss += bias;
            loss = j.second * loss;
            if(loss<=0){
                for(int k=0;k<feature_num;k++){
                    w[k] = w[k] + lr * j.first[k] * j.second;
                }
                bias = bias + lr * j.second;
            }
        }
    }
}

int main(){
    // nomal method
    // dataset file path 
    const string trainSet_path = "./train.txt";
    const string testSet_path = "./test.txt"; 
    // dataset initial and read
    vector<pair<vector<double>, double>> train_dataset;
    vector<pair<vector<double>, double>> test_dataset;
    // read train dataset
    Read_trainDataset(train_dataset, trainSet_path, 2);
    Read_testDataset(test_dataset, testSet_path, 2);
    cout<<"train dataset: "<<endl;
    for(auto i : train_dataset){
        cout<< "feature: ";
        for(auto j: i.first){
            cout<<j<<" ";
        }
        cout<<" label :";
        cout<<i.second<<endl;
    }
    cout<<"test dataset: "<<endl;
    for(auto i : test_dataset){
        cout<< "feature: ";
        for(auto j: i.first){
            cout<<j<<" ";
        }
        cout<<" label :";
        cout<<i.second<<endl;
    }
    vector<double> predict_data;
    // initial weight and bias as 0
    vector<double> w;
    double bias = 0.0f;
    w.push_back(0);
    w.push_back(0);
    // train
    double lr = 1.0f;
    training(train_dataset, 2, w, bias, lr, 8);
    cout<<"w: ";
    for(auto i : w){
        cout<<i<<" ";
    }
    cout<<endl;
    cout<<"b: "<<bias<<endl;
    return 0;
}
#endif 
