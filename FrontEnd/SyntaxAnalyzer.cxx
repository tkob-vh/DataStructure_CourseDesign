/********************************************************************
* FILE: SyntaxAnalyzer.cxx
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/

#include<string.h>
#include "GlobalVar.h"
#include "SyntaxAnalyzer.h"
#include "tree.hh"

extern char temp_kind[20];
extern char temp_text[20];
//extern tree<std::string>::iterator root;
extern tree<std::string> program_tree;
extern tree<std::string>::iterator program_root;

extern tree<std::string> external_defination_list_tree;
extern tree<std::string>::iterator external_defination_list_root;

extern tree<std::string> external_variable_defination_tree;
extern tree<std::string>::iterator external_variable_defination_root;

extern tree<std::string> function_defination_tree;
extern tree<std::string>::iterator function_defination_root;


bool LookupCapitalKeywords(char* w){
    for(int i=0;i<14;i++){
        if(strcmp(w,Capital_Keywords[i])==0){
            return true;
        }
    }
    return false;
}


void readtoken(){
    strcpy(w, t_kinds[token_counter1]);
    token_counter1++;
}

//the function used to operate the program
tree<std::string>::iterator program(){
    tree<std::string>::iterator root;
    readtoken();
    external_defination_list_root = external_defination_list();
    if(external_defination_list_root!=NULL){
        program_tree.set_head("program");
        root = program_tree.begin();
        program_tree.prepend_child(root,external_defination_list_root);
        return root;       
    }
    else{
        printf("Error in line %d : program should be external_defination_list\n",line_info[token_counter1-1]);
        return NULL;
    }
}

//the function used to operate the external_defination_list    need to be modified
tree<std::string>::iterator external_defination_list(){
    tree<std::string>::iterator root;
    tree<std::string> tmp_tree;
    //
    //
    //
    tmp_tree.set_head("external_defination_list");
    root = tmp_tree.begin();
    tmp_tree.prepend_child(root,external_defination());
    //external_defination_list_tree.insert_subtree(external_defination_list_root, external_defination());
    tmp_tree.append_child(root,external_defination_list());
    //external_defination_list_tree.insert_subtree(external_defination_list_root, external_defination_list());
    //copy!!!
    return root;
}

//the function used to operate the external_defination
tree<std::string>::iterator external_defination(){
    tree<std::string>::iterator root;
    if(!LookupCapitalKeywords(w)){
        printf("Error in line %d : external_defination should be function_defination or external_variable_defination\n",line_info[token_counter1-1]);
        return NULL;
    }
    else{   
        strcpy(temp_kind,w);
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : external_defination should be function_defination or external_variable_defination\n",line_info[token_counter1-1]);
            return NULL;
        }
        strcpy(temp_text,token_text[token_counter1-1]);
        readtoken();
        if(strcmp(w,"LP")){
            root=external_variable_defination();
        }
        else{
            root= function_defination();
        }
        if(root!=NULL)  return root;
    }
}

//the function used to operate the external_variable_defination
tree<std::string>::iterator external_variable_defination(){
    tree<std::string>::iterator root;
    external_variable_defination_tree.set_head("external_variable_defination");
    root = external_variable_defination_tree.begin();
    external_variable_defination_tree.prepend_child(root,temp_kind);
    auto p=external_variable_defination_tree.append_child(root,"external_variable_list");
    external_variable_defination_tree.prepend_child(p,temp_text);
    readtoken();
    if(strcmp(w,"COMMA") && strcmp(w,"SEMICOLON")){
        printf("Error in line %d : external_variable_defination should be external_variable_list SEMICOLON\n",line_info[token_counter1-1]);
        return NULL;
    }
    else if(!strcmp(w,"SEMICOLON")){
        readtoken();
        return root;
    }
    else{
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : external_variable_defination should be external_variable_list SEMICOLON\n",line_info[token_counter1-1]);
            external_variable_defination_tree.clear();
            return NULL;
        }
        
    }

}