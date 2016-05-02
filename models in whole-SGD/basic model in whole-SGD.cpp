//
//  main.cpp
//  test basic model
//
//  Created by Vanellope on 4/22/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//

#define MAX 2000
#define K 50//隐维度 10-100
#define M 943
#define N 1682
#define lamda 0.01//正则项系数
#define Gen 500


#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;


double gama=0.0001; //学习速率
double P[MAX][MAX];//M*K
double Q[MAX][MAX];//K*N
double e[MAX][MAX];//M*N
double er[MAX][MAX];//M*N
double P_1[MAX][MAX];//M*K
double Q_1[MAX][MAX];//K*N
int R[10][MAX][MAX]={0};//M*N


FILE *fp1=fopen("/Users/vanellope/Desktop/毕业论文代码/ml-100k/u2.base","r");
FILE *fp2=fopen("/Users/vanellope/Desktop/毕业论文代码/ml-100k/u2.test","r");


double randdouble(){
    double t;
    t=rand()/(double)RAND_MAX;
    return t;
}

void input(FILE* fp,int s){//第s个文件
    int i,j,rate,time;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            R[s][i][j]=0;
            e[i][j]=0;
            er[i][j]=0;
        }
    }
    while(fscanf(fp,"%d %d %d %d",&i,&j,&rate,&time)>0){
        R[s][i][j]=rate;//R1是训练，R2是测试
        //cout<<i<<' '<<j<<' '<<rate<<'\n';
    }
    return ;
}

void initial(){//采用了随机初始化
    for (int i=1;i<=M;i++){
        for(int j=1;j<=K;j++){
            P[i][j]=randdouble()*sqrt(3.0/K);
            //cout<<P[i][j]<<'\n';
        }
    }
    for (int i=1;i<=K;i++){
        for (int j=1;j<=N;j++){
            Q[i][j]=randdouble()*sqrt(3.0/K);
        }
    }
    return ;
}

void evaluate(){
    double count=0;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            er[i][j]=R[2][i][j];
            if (R[2][i][j]>0){
                count++;
                for (int k=1;k<=K;k++){
                    er[i][j]-=P[i][k]*Q[k][j];
                }
            }
        }
    }
    double sum=0;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            sum+=pow(er[i][j],2.0);
        }
    }
    sum=sqrt(sum/count);
    cout<<sum<<'\n';
    return ;
}

void matrixf(){
    double pre=0;
    for (int t=1;t<=Gen;t++){
        if(t>10) gama=0.0001;
        double loss=0;
        for (int i=1;i<=M;i++){
            for (int j=1;j<=N;j++){
                if (R[1][i][j]>0){
                    e[i][j]=R[1][i][j];
                    for (int k=1;k<=K;k++)
                        e[i][j]=e[i][j]-P[i][k]*Q[k][j];
                    /* for(int k=1;k<=K;k++){
                        double A=P[i][k]+gama*(e[i][j]*Q[k][j]-lamda*P[i][k]),
                        B=Q[k][j]+gama*(e[i][j]*P[i][k]-lamda*Q[k][j]);
                        P[i][k]=A;
                        Q[k][j]=B;
                    }*/
                }
            }
        }

        for(int i=1;i<=M;i++)
            for(int k=1;k<=K;k++){
                P_1[i][k]=P[i][k];
                for(int j=1;j<=N;j++)
                    P_1[i][k]+=gama*(e[i][j]*Q[k][j]-lamda*P[i][k]);
                }
        for(int j=1;j<=N;j++)
            for(int k=1;k<=K;k++){
                Q_1[k][j]=Q[k][j];
                for(int i=1;i<=M;i++)
                    Q_1[k][j]+=gama*(e[i][j]*P[i][k]-lamda*Q[k][j]);
            }
        for(int i=1;i<=M;i++)
            for(int k=1;k<=K;k++){
                P[i][k]=P_1[i][k];
                }
        for(int j=1;j<=N;j++)
            for(int k=1;k<=K;k++){
                Q[k][j]=Q_1[k][j];
                }
        for (int i=1;i<=M;i++){
            for (int j=1;j<=N;j++){
                if (R[1][i][j]>0){
                    loss+=pow(e[i][j],2);
                    //        cout<<loss<<'\n';
                    for (int k=1;k<=K;k++){
                        loss+=lamda*(pow(P[i][k],2)+pow(Q[k][j],2));
                    }
                }
                //cout<<e[i][j]<<'\n';
            }
        }
        if (t>=2){
            //cout<<pre<<' '<<loss<<endl;
            if (pre<loss)
                gama=gama/2.0;
         //   if((pre-loss)/pre>0.1)
           //     gama=gama*2;
            //  if (fabs(pre-loss)<=1) break;
        }
        pre=loss;
        // cout<<t<<'\n';
        if (t%50==0||1)
            cout<<"loss is "<<loss<<'\n';
     //   cout<<gama<<endl;
        if (t%50==0) evaluate();
    }
    return ;
}



int main(){
    input(fp1,1);
    input(fp2,2);
    initial();
    matrixf();
    //evaluate();
    system("pause");
    return 0;
}