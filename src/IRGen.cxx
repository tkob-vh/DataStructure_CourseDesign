/********************************************************************
* FILE: Scanner.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/


#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include"var.hh"
#include"tree.hh"
#include"IRGen.hh"
#include"Scanner.hh"
#include"Parser.hh"





//This function is used to generate the IR code
bool program_code(){
    FILE *fp;
    fp=fopen("./out/intermediate_code.cxx","w");
    if(fp==NULL){
        printf("Error in codeGen: cannot open the file\n");
        return false;
    }

    it=program_tree.begin();
    if(*it!="program"){
        printf("Error in codeGen: program_tree.begin() is not program\n");
        fclose(fp);
        return false;
    }
    it++;
    if(*it=="preprocessing_list"){
       preprocessing_code(fp);
    }
    if(*it=="external_defination_list"){
        external_defination_list_code(fp);
    }


    fclose(fp);
    return true;
}

//This function is used to generate the preprocessing code
//The preprocessing code begins with "preprocessing_list" and ends with "external_defination_list"
bool preprocessing_code(FILE *fp){
    while(*it!="external_defination_list"){
        while(*it!="preprocessing"){
            it++;
        }
        for(tree<std::string>::iterator it1=program_tree.begin(it);it1!=program_tree.end(it);it1++){
            fprintf(fp,"%s ",(*it1).c_str());
        }
        fprintf(fp,"\n");
    }
}


bool external_defination_code(FILE *fp){
    int counter;
    while(counter<token_counter1){
        if(!strcmp(token_text[counter],";")||!strcmp(token_text[counter],"}")){
            fprintf(fp,"%s\n",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],"{")){
            fprintf(fp,"\n%s\n",token_text[counter]);
        }
        else if(isFunction(token_text[counter])){
            fprintf(fp,"%s",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],"EOF")){
            1;
        }
        else{
             fprintf(fp,"%s ",token_text[counter]);
        }   
        counter++;
    }
    return true;
}
