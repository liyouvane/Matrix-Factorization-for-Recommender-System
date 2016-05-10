//
//  main.cpp
//  analysis on dataset
//
//  Created by Vanellope on 5/9/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//

#define MAX 2000

#define K 100//隐维度 10-100
//#define M 5
#define M 943
//#define N 4
#define N 1682
double gama=0.0008; //学习速率
#define lamda 0.01//正则项系数
#define Gen 2000
//#define Gen 50000


#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

double sum=0;
double miu;//average of all the ratings
double bu[MAX];//M
double bi[MAX];//N
double bu1[MAX];//M
double bi1[MAX];//N

double P[MAX][MAX];//M*K
double Q[MAX][MAX];//K*N
double P1[MAX][MAX];//M*K
double Q1[MAX][MAX];//K*N

int ker_u[10]={0};
int ker_i[10]={0};

double e[MAX][MAX];//M*N
double er[MAX][MAX];//M*N
int R[5][MAX][MAX]={0};//M*N
int T[MAX][MAX]={0};
int T1[MAX][MAX]={0};//T 转置

int num_u[MAX]={0};
int num_i[MAX]={0};

int type_u[MAX]={0};
int type_i[MAX]={0};
int num_type_u[5]={0};
int num_type_i[5]={0};

int dis_u[MAX][MAX]={0};
int dis_i[MAX][MAX]={0};
//FILE *fp1=fopen("/Users/vanellope/Desktop/毕业论文代码/ml-100k/test.in","r");
FILE *fp1=fopen("/ml-100k/u1.base","r");
FILE *fp2=fopen("/ml-100k/u1.test","r");

void input(FILE*fp,int s){
    int i,j,rate,time;
    while(fscanf(fp,"%d %d %d %d",&i,&j,&rate,&time)>0){
        R[s][i][j]=rate;//R1是训练，R2是测试
        if (s==1) {
            T[i][j]=1;
            T1[j][i]=1;
        }
    }
    return ;
}

void Quicksort(int a[], int left, int right){
    int i=left;
    int j=right;
    if (left>=right) return;
    int temp=a[left];
    while(i!=j){
        while(i<j&&a[j]>=temp) j--;
        if (i<j){
            a[i]=a[j];
            i++;
        }
        while(i<j&&a[i]<=temp) i++;
        if (i<j){
            a[j]=a[i];
            j--;
        }
    }
    a[i]=temp;
    Quicksort(a,left,i-1);
    Quicksort(a,i+1,right);
    return ;
}

void analysis(){
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            if (R[1][i][j]>0){
                num_u[i]++;
                num_i[j]++;
            }
        }
    }
    Quicksort(num_u,1,M);
    Quicksort(num_i,1,N);
    FILE *tp=fopen("analysis.out","w");
    for (int i=1;i<=M;i++){
        fprintf(tp,"%d %d\n",i,num_u[i]);
    }
    for (int j=1;j<=N;j++){
        fprintf(tp,"%d %d\n",j,num_i[j]);
    }
    return ;
}

int main(){
    input(fp1,1);
    analysis();
    return 0;
}