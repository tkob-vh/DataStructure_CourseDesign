/********************************************************************
* FILE: code_gen.cxx
* DESCRIPTION: 
* This file is used to generate the IR code. The output is stored in 
* the ./out/intermediate_code.cxx
* AUTHOR: yyx
* ID: U202215545
********************************************************************/


#include<stdio.h>
#include<time.h>
#include<string.h>
#include"var.hh"
#include"tree.hh"
#include"code_gen.hh"
#include"scanner.hh"
#include"parser.hh"




/********************************************************************
This function is used to generate the IR code
********************************************************************/
bool program_code(){
    FILE *fp;
    fp=fopen("./out/intermediate_code.cxx","w");
    if(fp==NULL){
        printf("Error in codeGen: cannot open the file\n");
        return false;
    }
    time_t t;
    time(&t);

    fprintf(fp,"/********************************************************************\n");
    fprintf(fp,"* FILE: intermediate_code.cxx\n");
    fprintf(fp,"* DESCRIPTION: \n");
    fprintf(fp,"* This file is used to store the IR code.\n");
    fprintf(fp,"* AUTHOR: yyx\n");
    fprintf(fp,"* ID: U202215545\n");
    fprintf(fp,"* TIME: %s",ctime(&t));
    fprintf(fp,"********************************************************************/\n\n\n");
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

    fprintf(fp,"\n\n\n");
    external_defination_list_code(fp);



    fclose(fp);
    return true;
}



/********************************************************************
This function is used to generate the preprocessing code
The preprocessing code begins with "preprocessing_list" and ends with "external_defination_list"
********************************************************************/
bool preprocessing_code(FILE *fp){
    while(1){
        while(*it!="preprocessing"){
            it++;
        }
        int child=program_tree.number_of_children(it);
        for(int i=0;i<child;i++){  
            fprintf(fp,"%s ",(*program_tree.child(it,i)).c_str());
        }
      
        fprintf(fp,"\n");
        tree<std::string>::iterator it1=program_tree.next_sibling(it);
        if(it1==NULL){

            return true;
        }
        else{
            it=it1;
        }
    }
    return true;
}



/********************************************************************
This function is used to generate the external defination code
The external defination code begins with "external_defination_list" and ends with "EOF"
********************************************************************/
bool external_defination_list_code(FILE *fp){
    bool for_tag=false;
    int counter=get_external_initial_counter();
    while(counter<token_counter1){
        if(!strcmp(token_text[counter],";")){
            fseek(fp,-1,SEEK_CUR);
            if(for_tag){
                fprintf(fp,"%s ",token_text[counter]);
            }
            else fprintf(fp,"%s\n%*s",token_text[counter],3*brace_counter,"");
        }
        else if(!strcmp(token_text[counter],"}")){
            brace_counter--;
            fseek(fp,-3,SEEK_CUR);
            fprintf(fp,"%s\n%*s",token_text[counter],3*brace_counter,"");

         }
        else if(!strcmp(token_text[counter],"{")){

            fprintf(fp,"\n%*s%s\n%*s",3*brace_counter,"",token_text[counter],3*brace_counter+3,"");
            brace_counter++;
        }
        else if(isFunction(token_text[counter])){
            fprintf(fp,"%s",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],")")){
            fseek(fp,-1,SEEK_CUR);
            if(for_tag) for_tag=false;
            fprintf(fp,"%s ",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],",")){
            fseek(fp,-1,SEEK_CUR);
            fprintf(fp,"%s",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],"[")){
            fseek(fp,-1,SEEK_CUR);
            fprintf(fp,"%s",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],"]")){
            fseek(fp,-1,SEEK_CUR);
            fprintf(fp,"%s ",token_text[counter]);
        }
        else if(!strcmp(token_text[counter],"EOF")){
            1;
        }
        else{
            if(!strcmp(token_text[counter-1],"(")){
                fseek(fp,-1,SEEK_CUR);
            }
            if(!strcmp(token_text[counter],"for")){
                for_tag=true;
            }
            fprintf(fp,"%s ",token_text[counter]);
        }   
        counter++;
    }
    return true;
}


/********************************************************************
This function is used to get the initial counter of the external defination
********************************************************************/
int get_external_initial_counter(){
    int t;
    for(t=0;t<token_counter1;t++){
        if(LookupKeyword(t)!=ID){
            if(strcmp(token_text[t],"define")&&strcmp(token_text[t],"include")){
                return t;
            }
        }
    }
    return -1;

}

