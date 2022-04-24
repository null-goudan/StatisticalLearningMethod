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

/* Information gain algorithm
    input: a dataset D and feature A
    output: the information gain g(D|A) of feature A for dataset
*/
class Data{
    public:
        vector<int> X;
        int label;
};

double InformationGain(vector<Data>& Dataset, int feature_A){
    // (1) calc the empirical entorpy of dataset D :  H(D)
    multiset<int> m_Y;
    set<int> Y_set;
    multiset<int> m_X;
    set<int> X_set;
    map<int, map<int, int>> m;
    
    double H_D = 0.0;
    double H_D_A = 0.0;
    for(auto i: Dataset){
        Y_set.insert(i.label);
        m_Y.insert(i.label);
        m_X.insert(i.X[feature_A]);
        X_set.insert(i.X[feature_A]);
        m[i.X[feature_A]][i.label] ++;
    }
    for(auto c: Y_set){
        H_D -= (m_Y.count(c)*1.0f/m_Y.size()) * log2f64(m_Y.count(c)*1.0f/m_Y.size()) ;
    }
    //cout<<"H(D) = "<<H_D<<endl;

    // (2) calc empirical conditional entorpy of the feature A for dataset D :  H(D|A)
    for(auto i: X_set){
        double B_sum = 0.0;
        for(auto c: Y_set){
            //cout<<m[i][c]<<" "<<m_X.count(i)<<endl;
            if(m[i][c]){
                B_sum += (m[i][c]*1.0f/m_X.count(i))*log2f64(m[i][c] * 1.0f / m_X.count(i));
            }
        }
        //cout<<"B_sum "<<B_sum<<endl;
        H_D_A -= (m_X.count(i)* 1.0f  / m_X.size() * 1.0f )*(B_sum);
    }
    //cout<<"H(D|A) = "<<H_D_A<<endl;

    // (2) calc the information gain g(D, A)

    //cout<<"g(D, A) = H(D) - H(D|A) = "<<H_D-H_D_A<<endl;
    return double(H_D-H_D_A);
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

int main(){
    const int feature_num = 4;
    vector<Data> Dataset;
    double max_res = 0.0;
    int index = -1;
    readDataset(Dataset, feature_num, "./train.txt");
    for(int i=0;i<feature_num;i++){
        double res = InformationGain(Dataset, i);
        cout << "the infomation gain of A is "<<res<<endl;
        if(max_res <= res){
            max_res = res;
            index = i;
        }
    }
    cout<<"The max infomation gain is:"<<max_res<<endl;
    cout<<"The best choose is Feature:"<<index+1<<endl;
    return 0;
}