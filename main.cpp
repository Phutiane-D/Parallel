#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

vector <vector<double> >  QuickSort(vector <vector<double> > myPoint, int left, int right, int size){
    if(right > left){
        double v = myPoint[right][size];
        int i = left;
        int j = right;
        while(i < j){
            while(myPoint[i][size]< v){
                i++;
            }
            while(j > i && myPoint[j][size]>= v){
                j--;
            }
            if(j > i){
                vector <double> t = myPoint[i];
                myPoint[i]= myPoint[j];
                myPoint[j]= t;
            }
            else{
                vector <double> t = myPoint[i];
                myPoint[i] = myPoint[right];
                myPoint[right] = t;
            }
        }
        myPoint = QuickSort(myPoint, left, i-1,size);
        myPoint = QuickSort(myPoint,i+1,right,size);
    }
    return myPoint;
}

double ComputeDistance(vector<double>myPoints, vector<double> sample){
    double SquareSum = 0.0;

    for(int k = 0; k < myPoints.size()-1; k++){
        SquareSum += pow(myPoints[k] - sample[k], 2);
    }
    return sqrt(SquareSum);
}

int KNN(vector< vector<double> > DataSet, vector<double> TestingSet){

    double distance;
    int size = DataSet.size();
    int innerSize = DataSet[0].size();

    for(int i = 0; i < DataSet.size(); i++){
        distance = ComputeDistance(DataSet[i], TestingSet);
        DataSet[i].push_back(distance);
    }

    DataSet = QuickSort(DataSet, 0, size-1, innerSize);

    int refs[] = {0,0,0};

    for(int k = 0; k < 5; k++){
        if(DataSet[k][innerSize]-2 == 1){
            refs[0]++;
        }
        else if(DataSet[k][innerSize]-2 == 2){
            refs[1]++;
        }
        else if(DataSet[k][innerSize]-2 == 3){
            refs[2]++;
        }
    }
    int max = refs[0];
    int index = 0;

    for(int l = 1; l < 3; l++){
        if(refs[l] > max){
            max = refs[l];
            index = l;
        }
    }
    return index+1;
}


vector<double> split(string data, char delim){
    vector<double> splitted;
    string temp = "";
    for(char c : data){
        if(c != delim){
            temp += c;
        }
        else{
            splitted.push_back(stod(temp));
            temp = "";
        }
    }
    if(temp == "Iris-setosa"){
        splitted.push_back(1);
    }
    else if(temp == "Iris-versicolor"){
        splitted.push_back(2);
    }
    else if(temp == "Iris-virginica"){
        splitted.push_back(3);
    }

    return splitted;
}


int main() {

    ifstream thefile("/home/phutiane/Desktop/High Performance/Assignment/data.txt");
    vector<double> vec;
    vector< vector<double> > datavec;
    string data;

    while(getline(thefile, data)){
        vec = split(data,',');
        datavec.push_back(vec);
    }
    thefile.close();

    vector< vector<double> > testing { {6.2,2.5,3.1,1.1}, {5.1,3.5,1.4,0.3} };

    int classify = KNN(datavec, testing[0]);
    cout<< classify << endl;

    return 0;
}