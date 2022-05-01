#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "omp.h"

using namespace std;


int partition(vector< vector<double> > &Point, int left, int right, int size){
    double v = Point[right][size];
    int i = left;
    int j = right;
    while(i < j){
        while(Point[i][size]< v){
            i++;
        }
        while(j > i && Point[j][size]>= v){
            j--;
        }
        if(j > i){
            vector <double> t = Point[i];
            Point[i]= Point[j];
            Point[j]= t;
        }
        else{
            vector <double> t = Point[i];
            Point[i] = Point[right];
            Point[right] = t;
        }
    }

    return i;
}

void Seq_QSort(vector< vector<double> > &point, int left, int right, int size){
    if(left < right){
        int p = partition(point, left, right, size);
        Seq_QSort(point, left, p-1, size);
        Seq_QSort(point, p+1, right, size);
    }
}

void Par_Sect_QSort(vector< vector<double> > &point, int left, int right, int size, int thresh){
    int p;
    if(left < right){
        if((right - left) < thresh){
            Seq_QSort(point, left, right, size);
        }
        else{
            p = partition(point, left, right, size);
            #pragma omp parallel
            {
                #pragma omp sections firstprivate(left, right, p, point, thresh)
                {
                    #pragma omp section
                        Par_Sect_QSort(point, left, p-1, size, thresh);
                    #pragma omp section    
                        Par_Sect_QSort(point, p+1, right, size, thresh);    
                }
            }
        }
    }
}

void Par_Task_QSort(vector< vector<double> > &point, int left, int right, int size, int thresh){
    int p;
    if(left < right){

        if((right - left) < thresh){
            Seq_QSort(point, left, right, size);
        }
        else{
            p = partition(point, left, right, size);
            #pragma omp task shared(point)
                Par_Task_QSort(point, left, p-1, size, thresh);
            #pragma omp task shared(point)
                Par_Task_QSort(point, p+1, right, size, thresh);
        }
    }
}



/*double ComputeDistance(vector<double>myPoints, vector<double> sample){
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

    //DataSet = QuickSort(DataSet, 0, size-1, innerSize);

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
} */


int main() {

    // ifstream thefile("/home/phutiane/Desktop/High Performance/Assignment/data.txt");
    // vector<double> vec;
    // vector< vector<double> > datavec;
    // string data;

    // while(getline(thefile, data)){
    //     vec = split(data,',');
    //     datavec.push_back(vec);
    // }
    // thefile.close();

    vector< vector<double> > testing { {6.2,2.5,3.1,1.1}, {5.1,3.5,1.4,0.3}, {2.5,8.1,2.1,9.6},{7.2,6.1,5.4,8.3},{0.3,5.1,6.7,4.1}};
    vector< vector<double> > testing1 = testing;
    vector< vector<double> > testing2 = testing;

    cout<<"Original data"<<endl;
    for(int i = 0; i < 5; i++){
        for(int k = 0; k < 4; k++){
            cout<<testing[i][k]<<" ";
        }
        cout<<endl;
    }

    Seq_QSort(testing,0,4,3);
    cout<<"Serial implementation results"<<endl;
    for(int i = 0; i < 5; i++){
        for(int k = 0; k < 4; k++){
            cout<<testing[i][k]<<" ";
        }
        cout<<endl;
    }

    omp_set_nested(1);
    omp_set_num_threads(2); 
    Par_Sect_QSort(testing1,0,4,3,2);
    cout<<"Parallel with section implementation results"<<endl;
    for(int i = 0; i < 5; i++){
        for(int k = 0; k < 4; k++){
            cout<<testing1[i][k]<<" ";
        }
        cout<<endl;
    }

    #pragma omp parallel 
    {
        #pragma omp single nowait
        {
            Par_Task_QSort(testing2,0,4,3,2);
        }
    }
    
    cout<<"Parallel with task implementation results"<<endl;
    for(int i = 0; i < 5; i++){
        for(int k = 0; k < 4; k++){
            cout<<testing2[i][k]<<" ";
        }
        cout<<endl;
    }


    // int classify = KNN(datavec, testing[0]);
    // cout<< classify << endl;

    return 0;
}