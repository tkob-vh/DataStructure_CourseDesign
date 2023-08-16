/********************************************************************
* FILE: SyntaxAnalyzer.cxx
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "GlobalVar.h"
#include "SyntaxAnalyzer.h"
#include "tree.h"


bool program(){
    strcpy(w, t_kinds[token_counter1]);
    token_counter1++;
    if(external_defination_list()){

    }
    else{
        printf("Error : program should be external_defination_list\n");
        return false;
    }

}

bool external_defination_list(){
    if(!strcmp(w,"END_OF_FILE")){
        return false;
    }
    
}
