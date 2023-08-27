/***********************************************************************************************
 * FILE: TestRoutine.c
 * DESCRIPTION: 
 * This file is used to test the functions in the project.
 * AUTHOR: yyx
************************************************************************************************/

#include<stdio.h>
#define MAX 1000

//this line is used to test the line comment


int i;

void foo(int a,char b);


int main()
{
    int a,b,c,d,e,f,g;
    a=1;
    b=a+1;
    c=b-2;
    d=b*5;
    e=d/2;
    f=a&&b;
    g=a||b;
    float o,p,q;
    o=2.3;
    double x,y,z;
    c=3.4;
    char l,m,n;
    l='a';

    int arr1[10];
    float arr2[10];
    double arr3[10];
    char arr4[10];
    
    foo(1,'a');

    if(a==1){
        a=2;
        b=3;
    }
    else{
        a=3;
    }

    if(c==1){
        c=2;
        d=5+a;
    }
    
    while(a>1){
        a=a-1;
        b=b+1;
        if(b>5){
            b=b-5;
            continue;
        }   
    }

    for(a=1;a<10;a=a+1){
        b=b+1;
        c=c-1;
        if(b==1){
            break;
        }
        else{
            b=3;
        }
    }

}


void foo(int a,char b){
    return;
}