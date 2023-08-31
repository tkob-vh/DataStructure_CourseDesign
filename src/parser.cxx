/********************************************************************
* FILE: parser.cxx
* DESCRIPTION:
* This file is used to parse the source code and generate the abstract 
* syntax tree. The output file is in the ./out/tree.txt .
* AUTHOR: yyx
* ID: U202215545
********************************************************************/

#include<string.h>
#include "parser.hh"
#include<stack>
#include<time.h>
#include "var.hh"
#include "scanner.hh"
using std::stack;


//the function is used to judge whether the token w is a capital keyword ,such as INT,CHAR,etc.
bool LookupCapitalKeywords(char* w){
    for(int i=0;i<16;i++){
        if(strcmp(w,Capital_Keywords[i])==0){
            return true;
        }
    }
    return false;
}



//the function is used to read the next token,such as ID,INT_LITERAL,etc.`
void readtoken(){
    strcpy(w, t_kinds[token_counter1]);
    token_counter1++;
}


// void readtoNextLine(){
//     while(strcmp(w,"SEMICOLON")){
//         readtoken();
//     }
//     readtoken();
// }


void jumpLine(){
    int tk=token_counter1-1;
    int line_c=line_info[tk];
    while(line_info[tk]!=line_c){
        tk++;
    }
    token_counter1=tk;
}

//the function is used to judge whether the token w is an operator
bool isOperator(char* w){
    if(!strcmp(w,"PLUS")||!strcmp(w,"MINUS")||!strcmp(w,"MULTIPLY")||!strcmp(w,"DIVIDE")||!strcmp(w,"MOD")||!strcmp(w,"ASSIGN")||!strcmp(w,"EQ")||!strcmp(w,"NEQ")||!strcmp(w,"GREATER")||!strcmp(w,"LESS")||!strcmp(w,"GREATER_EQ")||!strcmp(w,"LESS_EQ")||!strcmp(w,"AND")||!strcmp(w,"OR")||!strcmp(w,"LP")||!strcmp(w,"RP")||!strcmp(w,"SHARP")){
        return true;
    }
    else{
        return false;
    }
}




//the function is used to judge whether the token_text w is a function, such as main.
bool isFunction(char* w){
    for(int i=0;i<function_counter;i++){
        if(!strcmp(w,function_list[i])){
            return true;
        }
    }
    return false;
}



//the function used to convert the capital operator str,such as PLUS, MINUS, to the enum type
Token_kind StrtoEnum(char * str){
    if(!strcmp(str,"PLUS")){
        return PLUS;
    }
    else if(!strcmp(str,"MINUS")){
        return MINUS;
    }
    else if(!strcmp(str,"MULTIPLY")){
        return MULTIPLY;
    }
    else if(!strcmp(str,"DIVIDE")){
        return DIVIDE;
    }
    else if(!strcmp(str,"MOD")){
        return MOD;
    }
    else if(!strcmp(str,"ASSIGN")){
        return ASSIGN;
    }
    else if(!strcmp(str,"EQ")){
        return EQ;
    }
    else if(!strcmp(str,"NEQ")){
        return NEQ;
    }
    else if(!strcmp(str,"GREATER")){
        return GREATER;
    }
    else if(!strcmp(str,"LESS")){
        return LESS;
    }
    else if(!strcmp(str,"GREATER_EQ")){
        return GREATER_EQ;
    }
    else if(!strcmp(str,"LESS_EQ")){
        return LESS_EQ;
    }
    else if(!strcmp(str,"AND")){
        return AND;
    }
    else if(!strcmp(str,"OR")){
        return OR;
    }
    else if(!strcmp(str,"LP")){
        return LP;
    }
    else if(!strcmp(str,"RP")){
        return RP;
    }
    else if(!strcmp(str,"SHARP")){
        return SHARP;
    }
    else{
        return ERROR_TOKEN;
    }
}




//the function used to get the number of the operator in the table
int getTableNum(const char * w){
    for(int i=0;i<17;i++){
        if(!strcmp(w,table_arr[i])){
            return i;
        }
    }
    return -1;
}


//the function used to compare the priority of two operators
char getOperatorPriority(const char* w1,char* w2){
    int p1=getTableNum(w1);
    int p2=getTableNum(w2);
    char t=op_table[p1][p2];
    if(t==' '){
        return 'e';
    }
    else{
        return t;
    }
}



//the function used to operate the hole program
tree<std::string>::iterator program(){ 
    tree<std::string>::iterator root;
    readtoken(); 
    root=program_tree.set_head("program");
    preprocessing_list_tree=preprocessing_list();
    program_tree.move_in_below(root,preprocessing_list_tree);
    external_defination_list_tree=external_defination_list();
    program_tree.move_in_below(root,external_defination_list_tree);
    return root;       
}


/********************************************************************
the function is used to operate the preprocessing_list, such as #include<stdio.h>, #define MAX 1000, etc.
the first token of preprocessing_list(#) is already read when the function is called
the subsequent token(# or else) is read when the function returns
********************************************************************/
tree<std::string> preprocessing_list(){
    if(strcmp(w,"SHARP")){
        return empty_tree;
    }
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    tree<std::string> p;
    tree<std::string> q;
    root=tmp_tree.set_head("preprocessing_list");
    p=preprocessing();
    tmp_tree.move_in_below(root,p);
    q=preprocessing_list();
    tmp_tree.move_in_below(root,q);  
    return tmp_tree;
}




/********************************************************************
the function used to operate the preprocessing
the first token of preprocessing is already read when the function is called
the subsequent token is read when the function returns
********************************************************************/
tree<std::string> preprocessing(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    root=tmp_tree.set_head("preprocessing");
    readtoken();
    if(!strcmp(w,"INCLUDE")){
        tmp_tree.append_child(root,"#include");
        readtoken();
        if(!strcmp(w,"LESS")){
            strcpy(temp_kind,"<");
            readtoken();
            strcat(temp_kind,token_text[token_counter1-1]);
            strcat(temp_kind,">");
            tmp_tree.append_child(root,temp_kind);
            readtoken();
            readtoken();
        }
        else if(!strcmp(w,"DOUBLE_QUOTE")){
            strcpy(temp_kind,"\"");
            readtoken();
            strcat(temp_kind,token_text[token_counter1-1]);
            strcat(temp_kind,"\"");
            tmp_tree.append_child(root,temp_kind);
            readtoken();
            readtoken();
        }
        else{
            printf("Error in line %d : the token after the \"include\" is not a < or a \" \n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
    }
    else if(!strcmp(w,"DEFINE")){
        tmp_tree.append_child(root,"#define");
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : the token after the \"define\" is not an identifier \n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        else{
            tmp_tree.append_child(root,token_text[token_counter1-1]);
            readtoken();
            if(strcmp(w,"INT_LITERAL") && strcmp(w,"FLOAT_LITERAL") && strcmp(w,"CHAR_LITERAL")){
                printf("Error in line %d : the third token of the preprocessing is not a literal \n",line_info[token_counter1-1]);
                jumpLine();
                return empty_tree;
            }
            else{
                tmp_tree.append_child(root,token_text[token_counter1-1]);
                readtoken();
            }
        }
    }
    else{
        printf("Error in line %d : the token after \"#\" is not \"include\" or \"define\" \n",line_info[token_counter1-1]);
        jumpLine();
        return empty_tree;
    }
    return tmp_tree;
}




/********************************************************************
the function used to operate the external_defination_list 
the first token of external_defination_list is already read when the function is called
********************************************************************/
tree<std::string> external_defination_list(){
    if(!strcmp(w,"END_OF_FILE")){   
        return empty_tree;
    }
    else if(!LookupCapitalKeywords(w)){
        printf("Error in line %d : the first token(i.e.,\"%s\") of the external defination list is not a keyword\n",line_info[token_counter1-1],token_text[token_counter1-1]);
        jumpLine();
        return empty_tree;
    }
    tree<std::string> p;
    tree<std::string> q;
    tree<std::string>::iterator root;
    tree<std::string> tmp_tree;
    
    root=tmp_tree.set_head("external_defination_list");
 
    p=external_defination();
    q=external_defination_list();
    tmp_tree.move_in_below(root,p);
    tmp_tree.move_in_below(root,q);

    return tmp_tree;
}




/********************************************************************
the function used to operate the external_defination
the first token of external_defination is already read when the function is called
when the function returns,the last token of external_defination is already read,while the subsequent token is not read
********************************************************************/
tree<std::string> external_defination(){
    tree<std::string>::iterator root;
    tree<std::string> tmp_tree;
    if(!LookupCapitalKeywords(w)){
        printf("Error in line %d : the first token(i.e., \"%s\") of the external defination is not a keyword\n",line_info[token_counter1-1],token_text[token_counter1-1]);
        jumpLine();
        return empty_tree;
    }
    else{   
        strcpy(temp_kind,w);
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : the second token(i.e., \"%s\") of the external defination is not an identifier\n",line_info[token_counter1-1],token_text[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        strcpy(temp_text,token_text[token_counter1-1]);
        readtoken();
        if(strcmp(w,"LP")){
            tmp_tree=external_variable_defination();
        }
        else{
            strcpy(function_list[function_counter++],temp_text);
            tmp_tree = function_defination();
        }
        if(!tmp_tree.empty())  return tmp_tree;
        else{
            printf("Error in line %d : the external defination is illegal\n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
    }
}




/********************************************************************
the function used to operate the external_variable_defination
the variable type and the variable name are already read when the function is called, and the COMMA or SEMICOLON is read, too.
********************************************************************/
tree<std::string> external_variable_defination(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=tmp_tree.set_head("external_variable_defination");
    tmp_tree.append_child(root,temp_kind);
    p=tmp_tree.append_child(root,"external_variable_list");
    tmp_tree.append_child(p,temp_text);   
    while(1){
        if(strcmp(w,"COMMA") && strcmp(w,"SEMICOLON")){
            printf("Error in line %d : undefined token(i.e., \"%s\") in external_variable_defination\n",line_info[token_counter1-1],token_text[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        else if(!strcmp(w,"SEMICOLON")){
            readtoken();
            return tmp_tree;
        }
        else{
            readtoken();
            if(strcmp(w,"ID")){
                printf("Error in line %d :\"%s\" is not an identifier in external_variable_defination\n",line_info[token_counter1-1],token_text[token_counter1-1]);
                jumpLine();
                tmp_tree.clear();
                return empty_tree;
            }
            else{
                q=tmp_tree.append_child(p,"external_variable_list");
                p=q;
                tmp_tree.append_child(p,token_text[token_counter1-1]);
                readtoken();
            }
        }
    }
}




/********************************************************************
the function used to operate the function_defination
the return type, the function name and the LP are already read when the function is called
********************************************************************/
tree<std::string> function_defination(){
    tree<std::string> tmp_tree;
    tree<std::string> tmp_tree1;
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    tree<std::string> m;
    tree<std::string> n;
    root=tmp_tree.set_head("function_defination");
    tmp_tree.append_child(root,temp_kind);
    q=tmp_tree.append_child(root,temp_text);
    m=formal_parameter_list();
    tmp_tree.move_in_below(q,m);
    readtoken();
    if(!strcmp(w,"SEMICOLON")){
        tmp_tree.append_child(root,"function_prototype_declaration");
        readtoken();
      
        return tmp_tree;
    }
    else if(!strcmp(w,"LBRACE")){
        n=compound_statement();
        tmp_tree.move_in_below(root,n);
        return tmp_tree;
    }
    else{
        printf("Error in line %d : Neither SEMICOLON nor LBRACE . \n",line_info[token_counter1-1]);
        jumpLine();
        tmp_tree.clear();
        return empty_tree;
    }
}




/********************************************************************
the function used to operate the formal_parameter_list
the LP is already read when the function is called
the RP is read when the function returns
********************************************************************/
tree<std::string> formal_parameter_list(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    tree<std::string> p;
    tree<std::string> q;
    readtoken();
    if(!strcmp(w,"RP")){
        return empty_tree;
    }
    else{
        root=tmp_tree.set_head("formal_parameter_list");
        p=formal_parameter();
        tmp_tree.move_in_below(root,p);
        q=formal_parameter_list();
        tmp_tree.move_in_below(root,q);
        return tmp_tree;
    }
}


/********************************************************************
the function used to operate the formal_parameter
the formal parameter type has already been read when the function is called 
the subsquent parameter type or RP is read when the function returns
********************************************************************/
tree<std::string> formal_parameter(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    root=tmp_tree.set_head("formal_parameter");
    if(!LookupCapitalKeywords(w)){
        printf("Error in line %d :the type of the formal parameter(i.e., \"%s\") is illegal \n",line_info[token_counter1-1],w);
        jumpLine();
        return empty_tree;
    }
    else{
        tmp_tree.append_child(root,w);
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d :the name of the formal parameter(i.e., \"%s\") is illegal \n",line_info[token_counter1-1],token_text[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        else{
            tmp_tree.append_child(root,token_text[token_counter1-1]);
            readtoken();
            if(!strcmp(w,"COMMA")){
                return tmp_tree;
            }
            else if(!strcmp(w,"RP")){
                token_counter1--;
                return tmp_tree;
            }
            else{
                printf("Error in line %d :undefined token(i.e., \"%s\") in the formal parameter list\n",line_info[token_counter1-1],token_text[token_counter1-1]);
                jumpLine();
                return empty_tree;
            }
        }
    }
}



/********************************************************************
the function used to operate the compound_statement
the LBRACE is already read when the function is called
the RBRACE and the next token are read when the function returns
********************************************************************/
tree<std::string> compound_statement(){
   
    tree<std::string> tmp_tree;
    tree<std::string> tmp_tree1;
    tree<std::string> tmp_tree2;
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=tmp_tree.set_head("compound_statement");
    readtoken();
    if(LookupCapitalKeywords(w)){
        tmp_tree1=local_variable_defination_list();
        tmp_tree.move_in_below(root,tmp_tree1);
       
    }
    tmp_tree2=statement_list();
    tmp_tree.move_in_below(root,tmp_tree2);
    if(!strcmp(w,"END_OF_FILE")){
        strcpy(w,"RBRACE");
        token_counter1--;
    }
    if(strcmp(w,"RBRACE")){
        printf("Error in line %d : the end of the compound statement(i.e.,\"%s\") is not a RBRACE .\n",line_info[token_counter1-1],token_text
        [token_counter1-1]);
        jumpLine();
        tmp_tree.clear();
        return empty_tree;
    }
    readtoken();
    return tmp_tree;
}



/********************************************************************
the function used to operate the local_variable_defination_list
the first token (ie. the type) of local_variable_defination_list is already read when the function is called
the subsequent token is read when the function returns
********************************************************************/
tree<std::string> local_variable_defination_list(){
    if(strcmp(w,"INT") && strcmp(w,"FLOAT") && strcmp(w,"CHAR")&& strcmp(w,"LONG")&& strcmp(w,"DOUBLE")){
        return empty_tree;
    }
    tree<std::string> tmp_tree;
    tree<std::string> p;
    tree<std::string> q;
    tree<std::string>::iterator root;

    root=tmp_tree.set_head("local_variable_defination_list");
    p=local_variable_defination();
    tmp_tree.move_in_below(root,p);
    q=local_variable_defination_list();
    tmp_tree.move_in_below(root,q);
    return tmp_tree;
}



/********************************************************************
the function used to operate the local_variable_defination
the first token (ie. the type) of local_variable_defination is already read when the function is called
the SEMICOLON and the subsequent token are read when the function returns
********************************************************************/
tree<std::string> local_variable_defination(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=tmp_tree.set_head("local_variable_defination");
    tmp_tree.append_child(root,w);
    readtoken();
    if(strcmp(w,"ID")){
        printf("Error in line %d : the name of the local variable(i.e., \"%s\") is illegal \n",line_info[token_counter1-1],token_text[token_counter1-1]);
        jumpLine();
        return empty_tree;
    }
    else{
        p=tmp_tree.append_child(root,"local_variable_list");
        q=tmp_tree.append_child(p,token_text[token_counter1-1]);
        readtoken();
        while(1){
            if(strcmp(w,"COMMA") && strcmp(w,"SEMICOLON") && strcmp(w,"LBRACKET")){
                printf("Error in line %d : undefined token(i.e., \"%s\") in local_variable_defination\n",line_info[token_counter1-1],token_text[token_counter1-1]);
                jumpLine();
                return empty_tree;
            }
            else if(!strcmp(w,"SEMICOLON")){
                readtoken();
                return tmp_tree;
            }
            else if(!strcmp(w,"LBRACKET")){
                readtoken();
                if(strcmp(w,"INT_LITERAL")){
                    printf("Error in line %d : the size of the array is not an integer literal\n",line_info[token_counter1-1]);
                    jumpLine();
                    return empty_tree;
                }
                else{
                    char temp[100];
                    strcpy(temp,token_text[token_counter1-3]);
                    strcat(temp,"[");
                    strcat(temp,token_text[token_counter1-1]);
                    strcat(temp,"]");
                    tmp_tree.replace(q,temp);
                    readtoken();//the RBRACKET
                    readtoken();//the SEMICOLON
                    readtoken();
                    return tmp_tree;
                }

            }
            else{
                readtoken();
                if(strcmp(w,"ID")){
                    printf("Error in line %d :\"%s\" is not an identifier in local_variable_defination\n",line_info[token_counter1-1],token_text[token_counter1-1]);
                    jumpLine();
                    return empty_tree;
                }
                else{
                    q=tmp_tree.append_child(p,"local_variable_list");
                    p=q;
                    tmp_tree.append_child(p,token_text[token_counter1-1]);
                    readtoken();
                }
            }
        }
    }
    
}



/********************************************************************
the function used to operate the statement_list
the first token of statement_list is already read when the function is called
the subsequent token is read when the function returns
********************************************************************/
tree<std::string> statement_list(){
    if(!strcmp(w,"RBRACE")){
        return empty_tree;
    }
    if(!strcmp(w,"END_OF_FILE")){
        return empty_tree;
    }
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root=NULL;
    tree<std::string> p;
    tree<std::string> q;

    p=statement();
    if(p.empty()){
        if(errors>0){
            printf("Error in line %d : undefined token(i.e., \"%s\") in the statement\n",line_info[token_counter1-1],token_text[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        else{
            return empty_tree;
        }
    }
    else{
        root=tmp_tree.set_head("statement_list");
        tmp_tree.move_in_below(root,p);
        q=statement_list();
        tmp_tree.move_in_below(root,q);
       

        return tmp_tree;
    }
   

}



/********************************************************************
the function used to operate the statement
the first token of statement is already read when the function is called
********************************************************************/
tree<std::string> statement(){
    tree<std::string> tmp_tree;
    tree<std::string> tmp_tree1;
    tree<std::string> tmp_tree2;
    tree<std::string>::iterator root;
    tree<std::string> p;
    tree<std::string> q;
    tree<std::string>::iterator r;
    tree<std::string>::iterator parent;
    if(!strcmp(w,"IF")){
      
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : the if condition does not begins with LP \n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        readtoken();
       
        tmp_tree1=expression(RP);//the if condition ,ended with RP
        readtoken();//read the { or the statement      
        p=compound_statement();//the if statement(maybe compound_statement)
        if(!strcmp(w,"ELSE")){
            readtoken();//read the { or the statement
            q=compound_statement();//the else statement(maybe compound_statement)
            root=tmp_tree.set_head("if-else_statement");
            tmp_tree.move_in_below(root,tmp_tree1);
            tmp_tree.move_in_below(root,p);
            tmp_tree.move_in_below(root,q);
         
            return tmp_tree;
        }
        else{
            root=tmp_tree.set_head("if_statement");
            tmp_tree.move_in_below(root,tmp_tree1);
            tmp_tree.move_in_below(root,p);
            // statement_tree.append_child(root,expression_root);
            // statement_tree.append_child(root,p);
            return tmp_tree;
        }
    }
    else if(!strcmp(w,"LBRACE")){
        tmp_tree=compound_statement();
        return tmp_tree;
    }
    else if(!strcmp(w,"WHILE")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : the while condition does not begin with \"(\"\n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        readtoken();
        tmp_tree1=expression(RP);//the while condition ,ended with RP
        readtoken();//read the { or the statement
        p=compound_statement();//the while statement(maybe compound_statement)
        root=tmp_tree.set_head("while_statement");
        tmp_tree.move_in_below(root,tmp_tree1);
        tmp_tree.move_in_below(root,p);
        // statement_tree.append_child(root,expression_root);
        // statement_tree.append_child(root,p);
        return tmp_tree;
    }
    else if(!strcmp(w,"FOR")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : the for condition does not begin with \"(\"\n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        readtoken();
        root=tmp_tree.set_head("for_statement");
        tree<std::string>::iterator m;
        m=tmp_tree.append_child(root,"for_condition_statement");
        p=expression(SEMICOLON);
        tmp_tree.move_in_below(m,p);
        readtoken();
        p=expression(SEMICOLON);
        tmp_tree.move_in_below(m,p);
        readtoken();
        p=expression(RP);
        tmp_tree.move_in_below(m,p);
       
        readtoken();//read the { or the statement
        q=compound_statement();//the for statement(maybe compound_statement)
        tmp_tree.move_in_below(root,q);
    
        return tmp_tree;
    }
    else if(!strcmp(w,"RETURN")){
    
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            if(!strcmp(w,"INT_LITERAL")||!strcmp(w,"FLOAT_LITERAL")||!strcmp(w,"CHAR_LITERAL")){
                tmp_tree1.set_head(token_text[token_counter1-1]);
            }
            else{
                 tmp_tree1=expression(SEMICOLON);
            }
            root=tmp_tree.set_head("return_statement");
            tmp_tree.move_in_below(root,tmp_tree1);
            readtoken();
            readtoken();
            return tmp_tree;
        }
        else{
            root=tmp_tree.set_head("return_statement");
            readtoken();
            return tmp_tree;
        }
    }
    else if(!strcmp(w,"BREAK")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            printf("Error in line %d : the break statement does not end with \";\"\n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        root=tmp_tree.set_head("break_statement");
        readtoken();
        return tmp_tree;
    }
    else if(!strcmp(w,"CONTINUE")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            printf("Error in line %d : the continue statement does not end with \";\"\n",line_info[token_counter1-1]);
            jumpLine();
            return empty_tree;
        }
        root=tmp_tree.set_head("continue_statement");
        readtoken();
        return tmp_tree;
    }
    else if(!strcmp(w,"LP")){
        tmp_tree=expression(RP);
        readtoken();
        return tmp_tree;
    }
    else if(!strcmp(w,"ID")){
        if(isFunction(token_text[token_counter1-1])){
            char temp[100];
                strcpy(temp,token_text[token_counter1-1]);
                readtoken();
            do{
                strcat(temp,token_text[token_counter1-1]);
                readtoken();
            }while(strcmp(w,"SEMICOLON"));
            readtoken();
            tmp_tree.set_head(temp);
            return tmp_tree;
        }
        else{
             tmp_tree=expression(SEMICOLON);
            readtoken();
            return tmp_tree;
        }
       
    }
    else if(!(strcmp(w,"INT_LITERAL")&&strcmp(w,"FLOAT_LITERAL")&&strcmp(w,"CHAR_LITERAL")&&strcmp(w,"LONG_LITERAL")&&strcmp(w,"DOUBLE_LITERAL"))){
        tmp_tree=expression(SEMICOLON);
        readtoken();
        return tmp_tree;
    }
    else if(!strcmp(w,"RBRACE")){
        // readtoken();
        return empty_tree;
    }
    else if(!strcmp(w,"SEMICOLON")){
        readtoken();
        return statement();
    }
    else{
        errors++;
        return empty_tree;
    }
}



/********************************************************************
the function used to operate the expression, ended with endsym,which is semicolon or RP
the first token of expression is already read when the function is called
the subsequent token is not read when the function returns
********************************************************************/
tree<std::string> expression(int endsym){
    stack<std::string> operator_stack;
    stack< tree<std::string>::iterator > operand_stack;
    operator_stack.push("SHARP"); 
    
    tree<std::string> tmp_tree;
    tree<std::string> tmp_tree1;
    tree<std::string> tmp_node;
    tree<std::string>::iterator t1;
    tree<std::string>::iterator t2;

    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    
    int error=0;
    std::string t;
    p=tmp_node.set_head("node");
    //root=tmp_tree.begin();
   
    while(  ( strcmp(w,"SHARP") || operator_stack.top().compare("SHARP") )  && !error){
        if(!strcmp(w,"ID")||!strcmp(w,"INT_LITERAL")||!strcmp(w,"FLOAT_LITERAL")||!strcmp(w,"CHAR_LITERAL")||!strcmp(w,"LONG_LITERAL")||!strcmp(w,"DOUBLE_LITERAL")){
           if(isFunction(token_text[token_counter1-1])){
                char temp[100];
                strcpy(temp,token_text[token_counter1-1]);
                readtoken();
                do{
                    strcat(temp,token_text[token_counter1-1]);
                    readtoken();
                }while(strcmp(w,"RP"));
                strcat(temp,token_text[token_counter1-1]);//RP
                q=tmp_node.append_child(p,temp);
           }
            else{
                q=tmp_node.append_child(p,token_text[token_counter1-1]);
            }
     
            operand_stack.push(q);
        
            readtoken();
        }
        else if(isOperator(w)){
           
        
            switch(getOperatorPriority(operator_stack.top().c_str(),w)){
                case '>':
                  
                    t2=operand_stack.top();
                    operand_stack.pop();
                    t1=operand_stack.top();
                    operand_stack.pop();
                    t=operator_stack.top();
                    operator_stack.pop();
                    //generate the tree  


                    root=tmp_tree1.set_head(t);                   
                    tmp_tree1.append_child(root,t1);
                    tmp_tree1.append_child(root,t2);
                    tmp_tree=tmp_tree1;
                    root=tmp_tree.begin();
                    tmp_tree1.clear();
                    operand_stack.push(root);
               
                    break;
                case '<':
                   
                    operator_stack.push(w);
                    readtoken();
                    break;
                case '=':
                   
                    t=operator_stack.top();
                    operator_stack.pop();
                    readtoken();
                    break;
                default:
              
                    if(!strcmp(w,EnumtoStr(endsym))){
                        strcpy(w,"SHARP");
                    }
                    else{
                        error++;
                    }
            }
        }
        else if(!strcmp(w,EnumtoStr(endsym))){
            strcpy(w,"SHARP"); 
        }
        else{
            printf("Error in line %d :the token(i.e., \"%s\") in the expression is neither operand nor operator\n",line_info[token_counter1-1],token_text[token_counter1-1]);
            error++;
        }
        
    }
    if(operand_stack.size()==1&&!operator_stack.top().compare("SHARP")){
       
        return tmp_tree;
    }
    else{
        printf("Error in line %d : The current token is %s\n",line_info[token_counter1-1],token_text[token_counter1-1]);
        jumpLine();
        return empty_tree;
    }  
}



//the function used to traverse and print the tree
// void treePrint(){
//     FILE *fp;
//     fp=fopen("./out/tree1.txt","w");
//     if(fp==NULL){
//         printf("Error in opening the file tree.txt\n");
//         return;
//     }

//     tree< std::string >::iterator it;
//     int depth;
//     fprintf(fp,"\n\nHere is the Abstract Syntax Tree!!!\n\n");
//     for(it=program_tree.begin();it!=program_tree.end();++it){
//         depth=program_tree.depth(it);
//         if(program_tree.previous_sibling(it)==NULL)
//         fprintf(fp,"%*s├──%s\n",depth*2,"",(*it).c_str());
//         else
//         fprintf(fp,"%*s└──%s\n",depth*2,"",(*it).c_str());
//     }
// }




/********************************************************************
This function is used to print the AST
********************************************************************/
void printTree(){
    FILE *fp;
    fp=fopen("./out/tree.txt","w");
    if(fp==NULL){
        printf("Error in opening the file tree.txt\n");
        return;
    }
    time_t t;
    time(&t);
    fprintf(fp,"/********************************************************************\n");
    fprintf(fp,"* FILE: tree.txt\n");
    fprintf(fp,"* DESCRIPTION: \n");
    fprintf(fp,"* This is the output of the parser, which is the abstract syntax tree.\n");
    fprintf(fp,"* AUTHOR: yyx\n");
    fprintf(fp,"* ID: U202215545\n");
    fprintf(fp,"* TIME: %s",ctime(&t));
    fprintf(fp,"********************************************************************/\n\n");
    fprintf(fp,"Here is the Abstract Syntax Tree!!!\n\n");
    fprintf(fp,"%s\n",(*program_root).c_str());
    stack<tree<std::string>::iterator> s;
    s.push(program_root);
    while(s.top()!=program_tree.end()){
        tree<std::string>::iterator it=s.top();
        s.pop();
        for(tree<std::string>::iterator it1=program_tree.begin(it);it1!=program_tree.end(it);++it1){
            for(int i=1;i<program_tree.depth(it1);i++){
                tree<std::string>::iterator p1=program_tree.parent(it1);
                tree<std::string>::iterator p2=program_tree.parent(p1);
                if(program_tree.end(p2)==p1) fprintf(fp,"   ");
                else fprintf(fp,"│  ");
            }
            if(program_tree.depth(it1)>0){
                fprintf(fp,"├──");
            }
            fprintf(fp,"%s\n",(*it1).c_str());

            tree<std::string>::iterator it2=program_tree.end(it1);
            if(it2!=NULL){
                s.push(it2);
                it2=program_tree.previous_sibling(it2);
            }

        }
    }
}

