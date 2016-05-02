//
//  main.cpp
//  mf with item easy biases
//
//  Created by Vanellope on 4/20/16.
//  Copyright © 2016 Vanellope. All rights reserved.
//

#define MAX 2000
#define K 20//隐维度 10-100
//#define M 5
#define M 943
//#define N 4
#define N 1682
#define lamda 0.01//正则项系数
#define Gen 500
//#define Gen 50000


#include <iostream>
#include <math.h>
#include <cstdlib>
using namespace std;

double gama=0.001; //学习速率
double sum=0;
double miu;//average of all the ratings
double bu[MAX];//M
double bi[MAX];//N
double P[MAX][MAX];//M*K
double Q[MAX][MAX];//K*N
double P_1[MAX][MAX];//M*K
double Q_1[MAX][MAX];//K*N
double e[MAX][MAX];//M*N
double er[MAX][MAX];//M*N
int R[10][MAX][MAX]={0};//M*N


FILE *fp1=fopen("ml-100k/u1.base","r");
FILE *fp2=fopen("ml-100k/u1.test","r");

double randdouble(){
    double t;
    t=rand()/(double)RAND_MAX;
    return t;
}

void input(FILE* fp,int s){//第s个文件
    int i,j,rate,time;
    while(fscanf(fp,"%d %d %d %d",&i,&j,&rate,&time)>0){
        R[s][i][j]=rate;//R1是训练，R2是测试
    }
    return ;
}

void initial(){//采用了随机初始化
    long num[N]={0};
    for (int i=1;i<=M;i++){
        for(int j=1;j<=K;j++){
            P[i][j]=randdouble()*sqrt(3.0/K);
        }
    }
    for (int i=1;i<=K;i++){
        for (int j=1;j<=N;j++){
            Q[i][j]=randdouble()*sqrt(3.0/K);
        }
    }
    for (int j=1;j<=N;j++)
        bi[j]=0;
  //  cout<<bi[599]<<endl;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            e[i][j]=0;
            er[i][j]=0;
            bi[j]+=R[1][i][j];
           // if(j==599) cout<<bi[599]<<endl;
            if (R[1][i][j]>0)
                num[j]++;
        }
    }
    //cout<<num[599]<<endl;
    for (int j=1;j<=N;j++)
        if (num[j]==0) bi[j]=0;
        else bi[j]=bi[j]/num[j];
    //cout<<'1';
   // cout<<bi[599]<<endl;
    return ;
}

void evaluate(){
    int count=0;
  //  cout<<bi[599]<<endl;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++){
            if (R[2][i][j]>0){
                count++;
                
                er[i][j]=R[2][i][j]-bi[j];
              //  if(i==7&&j==599) cout<<R[2][i][j]<<' '<<bi[j]<<' '<<er[i][j]<<endl;
                for (int k=1;k<=K;k++){
                    er[i][j]=er[i][j]-P[i][k]*Q[k][j];
                }
            }
        }
    }
  //  cout<<er[7][599];
    double sumerror=0;
    for (int i=1;i<=M;i++){
        for (int j=1;j<=N;j++)
            if(R[2][i][j]>0)
            {
              //  if(i==7&&j==599) cout<<"FFFF "<<er[i][j]<<' '<<i<<' '<<j<<'\n';
                sumerror+=pow(er[i][j],2.0);
               // cout<<sumerror<<' '<<i<<' '<<j<<'\n';
                }
    }
    sumerror=sqrt(sumerror/count);
    cout<<sumerror<<'\n';
    return ;
}

void matrixf(){
    long double pre=0;
    for (int t=1;t<=Gen;t++){
        long double loss=0;
        for (int i=1;i<=M;i++){
            for (int j=1;j<=N;j++){
                if (R[1][i][j]>0){
                    e[i][j]=R[1][i][j]-bi[j];
                    for (int k=1;k<=K;k++)
                        e[i][j]=e[i][j]-P[i][k]*Q[k][j];
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
                    loss+=pow(e[i][j],2)+lamda*pow(bi[j],2.0);
                    for (int k=1;k<=K;k++){
                        loss+=lamda*(pow(P[i][k],2)+pow(Q[k][j],2));
                    }
                }
                //cout<<e[i][j]<<'\n';
            }
        }
        if (t>=2){
            if (pre<loss)
                gama=gama/2.0;
            //if (fabs(pre-loss)<=1) break;
        }
        pre=loss;
        if (loss<0.0001) break;
        // cout<<t<<'\n';
        //if (t%50==0) cout<<"Gen "<<t<<' '<<"loss is "<<loss<<'\n';
        if (t%50==0||1) cout<<"loss is "<<loss<<'\n';
        if (t%50==0) evaluate();
    }
  //  cout<<bi[599]<<endl;
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