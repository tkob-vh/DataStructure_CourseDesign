/********************************************************************
* FILE: Parser.cxx
* DESCRIPTION:
* wasted a lot of time searching the library of tree.hh
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/

#include<string.h>
#include "GlobalVar.h"
#include "Parser.h"
#include "tree.hh"

extern char temp_kind[20];
extern char temp_text[20];
extern int errors;
//extern tree<std::string>::iterator root;
extern tree<std::string> program_tree;
extern tree<std::string>::iterator program_root;

extern tree<std::string> external_defination_list_tree;
extern tree<std::string>::iterator external_defination_list_root;

extern tree<std::string> external_variable_defination_tree;
extern tree<std::string>::iterator external_variable_defination_root;

extern tree<std::string> function_defination_tree;
extern tree<std::string>::iterator function_defination_root;

extern tree<std::string> compound_statement_tree;
extern tree<std::string>::iterator compound_statement_root;

extern tree<std::string> local_variable_defination_list_tree;
extern tree<std::string>::iterator local_variable_defination_list_root;

extern tree<std::string> statement_list_tree;
extern tree<std::string>::iterator statement_list_root;

extern tree<std::string> statement_tree;
extern tree<std::string>::iterator statement_root;

extern tree<std::string> expression_tree;
extern tree<std::string>::iterator expression_root;

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
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    external_variable_defination_tree.set_head("external_variable_defination");
    root = external_variable_defination_tree.begin();
    external_variable_defination_tree.prepend_child(root,temp_kind);
    p=external_variable_defination_tree.append_child(root,"external_variable_list");
    external_variable_defination_tree.prepend_child(p,temp_text);   
    while(1){
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
            else{
                q=external_variable_defination_tree.append_child(p,"external_variable_list");
                p=q;
                external_variable_defination_tree.prepend_child(p,token_text[token_counter1-1]);
                readtoken();
            }
        }
    }
}


//the function used to operate the function_defination
tree<std::string>::iterator function_defination(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    function_defination_tree.set_head("function_defination");
    root = function_defination_tree.begin();
    p=function_defination_tree.prepend_child(root,temp_kind);
    q=function_defination_tree.insert_after(p,temp_text);
    function_defination_tree.prepend_child(q,formal_parameter_list());
    readtoken();
    if(!strcmp(w,"SEMICONLON")){
        function_defination_tree.append_child(root,"function_prototype_declaration");
        return root;
    }
    else if(!strcmp(w,"LBRACE")){
        function_defination_tree.append_child(root,compound_statement()); 
        return root;
    }
    else{
        printf("Error in line %d : function_defination should be formal_parameter_list SEMICOLON or compound_statement\n",line_info[token_counter1-1]);
        function_defination_tree.clear();
        return NULL;
    }
}

//the function used to operate the compound_statement
tree<std::string>::iterator compound_statement(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    compound_statement_tree.set_head("compound_statement");
    root = compound_statement_tree.begin();
    readtoken();
    if(LookupCapitalKeywords(w)){
        local_variable_defination_list_root=local_variable_defination_list();
        compound_statement_tree.prepend_child(root,local_variable_defination_list_root);
    }
    statement_list_root=statement_list();
    compound_statement_tree.append_child(root,statement_list_root);
    if(strcmp(w,"RBRACE")){
        printf("Error in line %d : compound_statement should be LBRACE local_variable_defination_list statement_list RBRACE\n",line_info[token_counter1-1]);
        compound_statement_tree.clear();
        return NULL;
    }
    readtoken();
    return root;
}

//the function used to operate the statement_list
tree<std::string>::iterator statement_list(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root=NULL;
    tree<std::string>::iterator p;
    p=statement();
    if(p==NULL){//need to be modified
        printf("Error in line %d : statement_list should be statement\n",line_info[token_counter1-1]);
        return NULL;
    }
    else{
        tmp_tree.set_head("statement_list");
        root = tmp_tree.begin();
        tmp_tree.prepend_child(root,p);
        tmp_tree.append_child(root,statement_list());
        statement_root=p;
        //copy tmp_tree to statement_list_tree!!!
        return root;
    }

}

//the function used to operate the statement
tree<std::string>::iterator statement(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    tree<std::string>::iterator r;
    if(!strcmp(w,"IF")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : statement should be IF LP expression RP statement\n",line_info[token_counter1-1]);
            return NULL;
        }
        expression_root=expression();//the if condition ,ended with RP
        readtoken();//read the { or the statement
        p=statement();//the if statement(maybe compound_statement)
        if(!strcmp(w,"ELSE")){
            readtoken();//read the { or the statement
            q=statement();//the else statement(maybe compound_statement)
            statement_tree.set_head("if-else_statement");
            root = statement_tree.begin();
            statement_tree.append_child(root,expression_root);
            statement_tree.append_child(root,p);
            statement_tree.append_child(root,q);
            return root;
        }
        else{
            statement_tree.set_head("if_statement");
            root = statement_tree.begin();
            statement_tree.append_child(root,expression_root);
            statement_tree.append_child(root,p);
            return root;
        }
    }
    else if(!strcmp(w,"LBRACE")){
        root=compound_statement();
        return root;
    }
    else if(!strcmp(w,"WHILE")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : statement should be WHILE LP expression RP statement\n",line_info[token_counter1-1]);
            return NULL;
        }
        expression_root=expression();//the while condition ,ended with RP
        readtoken();
        p=statement();//the while statement(maybe compound_statement)
        statement_tree.set_head("while_statement");
        root = statement_tree.begin();
        statement_tree.append_child(root,expression_root);
        statement_tree.append_child(root,p);
        return root;
    }
    else if(!strcmp(w,"RETURN")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            expression_root=expression();
            readtoken();
            if(strcmp(w,"SEMICOLON")){
                printf("Error in line %d : statement should be RETURN SEMICOLON or RETURN expression SEMICOLON\n",line_info[token_counter1-1]);
                return NULL;
            }
            statement_tree.set_head("return_statement");
            root = statement_tree.begin();
            statement_tree.append_child(root,expression_root);
            readtoken();
            return root;
        }
        else{
            statement_tree.set_head("return_statement");
            root = statement_tree.begin();
            readtoken();
            return root;
        }
    }
    else if(!strcmp(w,"BREAK")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            printf("Error in line %d : statement should be BREAK SEMICOLON\n",line_info[token_counter1-1]);
            return NULL;
        }
        statement_tree.set_head("break_statement");
        root = statement_tree.begin();
        readtoken();
        return root;
    }
    else if(!strcmp(w,"CONTINUE")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            printf("Error in line %d : statement should be CONTINUE SEMICOLON\n",line_info[token_counter1-1]);
            return NULL;
        }
        statement_tree.set_head("continue_statement");
        root = statement_tree.begin();
        readtoken();
        return root;
    }
    else if(!strcmp(w,"LP")){
        root=expression();
        readtoken();
        return root;
    }
    else if(!strcmp(w,"ID")){
        
    }
    else if(!(strcmp(w,"INT_LITERAL")&&strcmp(w,"FLOAT_LITERAL")&&strcmp(w,"CHAR_LITERAL"))){
        root=expression();
        readtoken();
        return root;
    }
    else if(!strcmp(w,"RBRACE")){
        readtoken();
        return NULL;
    }
    else{
        errors++;
        printf("Error in line %d : statement should be IF LP expression RP statement or LBRACE statement_list RBRACE or WHILE LP expression RP statement or RETURN SEMICOLON or RETURN expression SEMICOLON or BREAK SEMICOLON or CONTINUE SEMICOLON or expression SEMICOLON or ID LP argument_list RP SEMICOLON or ID ASSIGN expression SEMICOLON or SEMICOLON\n",line_info[token_counter1-1]);
        return NULL;
    }
}

//the function used to operate the expression
tree<std::string>::iterator expression(){
    
}