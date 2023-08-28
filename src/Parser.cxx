/********************************************************************
* FILE: Parser.cxx
* DESCRIPTION:
* 
* AUTHOR: yyx
********************************************************************/

#include<string.h>
#include "Parser.hh"
#include<stack>
#include "var.hh"
#include "Scanner.hh"
using std::stack;


//the function used to judge whether the token w is a capital keyword ,such as INT,CHAR,etc.
bool LookupCapitalKeywords(char* w){
    for(int i=0;i<14;i++){
        if(strcmp(w,Capital_Keywords[i])==0){
            return true;
        }
    }
    return false;
}


//the function used to read the next token,such as ID,INT_LITERAL,etc.`
void readtoken(){
    strcpy(w, t_kinds[token_counter1]);
    token_counter1++;
}


//the function used to judge whether the token w is an operator
bool isOperator(char* w){
    if(!strcmp(w,"PLUS")||!strcmp(w,"MINUS")||!strcmp(w,"MULTIPLY")||!strcmp(w,"DIVIDE")||!strcmp(w,"MOD")||!strcmp(w,"ASSIGN")||!strcmp(w,"EQ")||!strcmp(w,"NEQ")||!strcmp(w,"GREATER")||!strcmp(w,"LESS")||!strcmp(w,"GREATER_EQ")||!strcmp(w,"LESS_EQ")||!strcmp(w,"AND")||!strcmp(w,"OR")||!strcmp(w,"LP")||!strcmp(w,"RP")||!strcmp(w,"SHARP")){
        return true;
    }
    else{
        return false;
    }
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



//the function used to operate the program
tree<std::string>::iterator program(){  //need to add preprocessing_list
    tree<std::string>::iterator root;
    readtoken(); 
    root=program_tree.set_head("program");
    preprocessing_list_root=preprocessing_list();
    program_tree.append_child(root,preprocessing_list_root);
    external_defination_list_root = external_defination_list();
    program_tree.append_child(root,external_defination_list_root);
    return root;       
}


//the function used to operate the preprocessing_list
//the first token of preprocessing_list(#) is already read when the function is called
//the subsequent token(# or else) is read when the function returns
tree<std::string>::iterator preprocessing_list(){
    if(strcmp(w,"SHARP")){
        return NULL;
    }
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    root=tmp_tree.set_head("preprocessing_list");
    tmp_tree.append_child(root,preprocessing());
    tmp_tree.append_child(root,preprocessing_list());
    //preprocessing_list_tree=tmp_tree;
    return root;
}

//the function used to operate the preprocessing
//the first token of preprocessing is already read when the function is called
tree<std::string>::iterator preprocessing(){
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
        }
        else if(!strcmp(w,"DOUBLE_QUOTE")){
            strcpy(temp_kind,"\"");
            readtoken();
            strcat(temp_kind,token_text[token_counter1-1]);
            strcat(temp_kind,"\"");
            tmp_tree.append_child(root,temp_kind);
            readtoken();
        }
        else{
            printf("Error in line %d : the second token of the preprocessing is not a < or a \" \n",line_info[token_counter1-1]);
            return NULL;
        }
    }
    else if(!strcmp(w,"DEFINE")){
        tmp_tree.append_child(root,"#define");
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : the second token of the preprocessing is not an identifier \n",line_info[token_counter1-1]);
            return NULL;
        }
        else{
            tmp_tree.append_child(root,token_text[token_counter1-1]);
            readtoken();
            if(strcmp(w,"INT_LITERAL") && strcmp(w,"FLOAT_LITERAL") && strcmp(w,"CHAR_LITERAL")){
                printf("Error in line %d : the third token of the preprocessing is not a literal \n",line_info[token_counter1-1]);
                return NULL;
            }
            else{
                tmp_tree.append_child(root,token_text[token_counter1-1]);
                readtoken();
            }
        }
    }
    else{
        printf("Error in line %d : the first token of the preprocessing is not a # \n",line_info[token_counter1-1]);
        return NULL;
    }
    return root;
}


//the function used to operate the external_defination_list 
//the first token of external_defination_list is already read when the function is called
tree<std::string>::iterator external_defination_list(){
    if(!strcmp(w,"END_OF_FILE")){   
        return NULL;
    }
    tree<std::string>::iterator root;
    tree<std::string> tmp_tree;
    
    root=tmp_tree.set_head("external_defination_list");
    tmp_tree.append_child(root,external_defination());
    tmp_tree.append_child(root,external_defination_list());
    external_defination_list_tree=tmp_tree;
    return root;
}

//the function used to operate the external_defination
//the first token of external_defination is already read when the function is called
//when the function returns,the last token of external_defination is already read,while the subsequent token is not read
tree<std::string>::iterator external_defination(){
    tree<std::string>::iterator root;
    if(!LookupCapitalKeywords(w)){
        printf("Error in line %d : the first token of the external defination is not a keyword\n",line_info[token_counter1-1]);
        return NULL;
    }
    else{   
        strcpy(temp_kind,w);
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d : the second token of the external defination is not an identifier\n",line_info[token_counter1-1]);
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
        else{
            printf("Error in line %d : the external defination is illegal\n",line_info[token_counter1-1]);
            return NULL;
        }
    }
}



//the function used to operate the external_variable_defination
//the variable type and the variable name are already read when the function is called, and the COMMA or SEMICOLON is read, too.
tree<std::string>::iterator external_variable_defination(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=external_variable_defination_tree.set_head("external_variable_defination");
    external_variable_defination_tree.append_child(root,temp_kind);
    p=external_variable_defination_tree.append_child(root,"external_variable_list");
    external_variable_defination_tree.append_child(p,temp_text);   
    while(1){
        if(strcmp(w,"COMMA") && strcmp(w,"SEMICOLON")){
            printf("Error in line %d : undefined token in external_variable_defination\n",line_info[token_counter1-1]);
            return NULL;
        }
        else if(!strcmp(w,"SEMICOLON")){
            readtoken();
            return root;
        }
        else{
            readtoken();
            if(strcmp(w,"ID")){
                printf("Error in line %d : not an identifier in external_variable_defination\n",line_info[token_counter1-1]);
                external_variable_defination_tree.clear();
                return NULL;
            }
            else{
                q=external_variable_defination_tree.append_child(p,"external_variable_list");
                p=q;
                external_variable_defination_tree.append_child(p,token_text[token_counter1-1]);
                readtoken();
            }
        }
    }
}


//the function used to operate the function_defination
//the return type, the function name and the LP are already read when the function is called
tree<std::string>::iterator function_defination(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=function_defination_tree.set_head("function_defination");
    function_defination_tree.append_child(root,temp_kind);
    q=function_defination_tree.append_child(root,temp_text);
    function_defination_tree.append_child(q,formal_parameter_list());
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


//the function used to operate the formal_parameter_list
//the LP is already read when the function is called
//the RP is read when the function returns
tree<std::string>::iterator formal_parameter_list(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    readtoken();
    if(!strcmp(w,"RP")){
        return NULL;
    }
    else{
        root=tmp_tree.set_head("formal_parameter_list");
        tmp_tree.append_child(root,formal_parameter());
        tmp_tree.append_child(root,formal_parameter_list());
        formal_parameter_list_tree=tmp_tree;
        return root;
    }
}

//the function used to operate the formal_parameter
//the formal parameter type has already been read when the function is called 
//the subsquent parameter type or RP is read when the function returns
tree<std::string>::iterator formal_parameter(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    root=tmp_tree.set_head("formal_parameter");
    if(!LookupCapitalKeywords(w)){
        printf("Error in line %d :the type of the formal parameter is illegal \n",line_info[token_counter1-1]);
        return NULL;
    }
    else{
        tmp_tree.append_child(root,w);
        readtoken();
        if(strcmp(w,"ID")){
            printf("Error in line %d :the name of the formal parameter is illegal \n",line_info[token_counter1-1]);
            return NULL;
        }
        else{
            tmp_tree.append_child(root,token_text[token_counter1-1]);
            readtoken();
            if(!strcmp(w,"COMMA")){
                readtoken();
                return root;
            }
            else if(!strcmp(w,"RP")){
                return root;
            }
            else{
                printf("Error in line %d :undefined token in the formal parameter list\n",line_info[token_counter1-1]);
                return NULL;
            }
        }
    }
}


//the function used to operate the compound_statement
//the LBRACE is already read when the function is called
//the RBRACE and the next token are read when the function returns
tree<std::string>::iterator compound_statement(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=compound_statement_tree.set_head("compound_statement");
    readtoken();
    if(LookupCapitalKeywords(w)){
        local_variable_defination_list_root=local_variable_defination_list();
        compound_statement_tree.append_child(root,local_variable_defination_list_root);
    }
    statement_list_root=statement_list();
    compound_statement_tree.append_child(root,statement_list_root);
    if(strcmp(w,"RBRACE")){
        printf("Error in line %d : the end of the compound statement is not a RBRACE \n",line_info[token_counter1-1]);
        compound_statement_tree.clear();
        return NULL;
    }
    readtoken();
    return root;
}


//the function used to operate the local_variable_defination_list
//the first token (ie. the type) of local_variable_defination_list is already read when the function is called
//the subsequent token is read when the function returns
tree<std::string>::iterator local_variable_defination_list(){
    if(!LookupCapitalKeywords(w)){
        return NULL;
    }
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    root=tmp_tree.set_head("local_variable_defination_list");
    tmp_tree.append_child(root,local_variable_defination());
    tmp_tree.append_child(root,local_variable_defination_list());
    local_variable_defination_list_tree=tmp_tree;
    return root;
}


//the function used to operate the local_variable_defination
//the first token (ie. the type) of local_variable_defination is already read when the function is called
//the SEMICOLON and the subsequent token are read when the function returns
tree<std::string>::iterator local_variable_defination(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    root=tmp_tree.set_head("local_variable_defination");
    tmp_tree.append_child(root,w);
    readtoken();
    if(strcmp(w,"ID")){
        printf("Error in line %d : the name of the local variable is illegal \n",line_info[token_counter1-1]);
        return NULL;
    }
    else{
        p=tmp_tree.append_child(root,"local_variable_list");
        tmp_tree.append_child(p,token_text[token_counter1-1]);
        readtoken();
        while(1){
            if(strcmp(w,"COMMA") && strcmp(w,"SEMICOLON")){
                printf("Error in line %d : undefined token in local_variable_defination\n",line_info[token_counter1-1]);
                return NULL;
            }
            else if(!strcmp(w,"SEMICOLON")){
                readtoken();
                return root;
            }
            else{
                readtoken();
                if(strcmp(w,"ID")){
                    printf("Error in line %d : not an identifier in local_variable_defination\n",line_info[token_counter1-1]);
                    return NULL;
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



//the function used to operate the statement_list
//the first token of statement_list is already read when the function is called
//the subsequent token is read when the function returns
tree<std::string>::iterator statement_list(){
    tree<std::string> tmp_tree;
    tree<std::string>::iterator root=NULL;
    tree<std::string>::iterator p;
    p=statement();
    if(p==NULL){
        if(errors>0){
            printf("Error in line %d : statement_list should be statement\n",line_info[token_counter1-1]);
            return NULL;
        }
        else{
            return NULL;
        }
    }
    else{
        root=tmp_tree.set_head("statement_list");
        tmp_tree.append_child(root,p);
        tmp_tree.append_child(root,statement_list());
        statement_root=p;
        statement_list_tree=tmp_tree;
        return root;
    }

}

//the function used to operate the statement
//the first token of statement is already read when the function is called
tree<std::string>::iterator statement(){
    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    tree<std::string>::iterator q;
    tree<std::string>::iterator r;
    if(!strcmp(w,"IF")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : the if condition does not begins with LP \n",line_info[token_counter1-1]);
            return NULL;
        }
        expression_root=expression(RP);//the if condition ,ended with RP
        readtoken();//read the { or the statement
        p=statement_list();//the if statement(maybe compound_statement)
        if(!strcmp(w,"ELSE")){
            readtoken();//read the { or the statement
            q=statement_list();//the else statement(maybe compound_statement)
            root=statement_tree.set_head("if-else_statement");
            statement_tree.append_child(root,expression_root);
            statement_tree.append_child(root,p);
            statement_tree.append_child(root,q);
            return root;
        }
        else{
            root=statement_tree.set_head("if_statement");
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
        expression_root=expression(RP);//the while condition ,ended with RP
        readtoken();//read the { or the statement
        p=statement_list();//the while statement(maybe compound_statement)
        root=statement_tree.set_head("while_statement");
        statement_tree.append_child(root,expression_root);
        statement_tree.append_child(root,p);
        return root;
    }
    else if(!strcmp(w,"FOR")){
        readtoken();
        if(strcmp(w,"LP")){
            printf("Error in line %d : statement should be FOR LP expression SEMICOLON expression SEMICOLON expression RP statement\n",line_info[token_counter1-1]);
            return NULL;
        }
        readtoken();
        root=statement_tree.set_head("for_statement");
        p=statement_tree.append_child(root,"for_condition_statement");
        statement_tree.append_child(p,expression(SEMICOLON));
        statement_tree.append_child(p,expression(SEMICOLON));
        statement_tree.append_child(p,expression(RP));
        readtoken();//read the { or the statement
        q=statement_list();//the for statement(maybe compound_statement)
        statement_tree.append_child(root,q);
        return root;
    }
    else if(!strcmp(w,"RETURN")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            expression_root=expression(SEMICOLON);
            // if(strcmp(w,"SEMICOLON")){
            //     printf("Error in line %d : return statement should end up with SEMICOLON\n",line_info[token_counter1-1]);
            //     return NULL;
            // }
            root=statement_tree.set_head("return_statement");
            statement_tree.append_child(root,expression_root);
            readtoken();
            return root;
        }
        else{
            root=statement_tree.set_head("return_statement");
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
        root=statement_tree.set_head("break_statement");
        readtoken();
        return root;
    }
    else if(!strcmp(w,"CONTINUE")){
        readtoken();
        if(strcmp(w,"SEMICOLON")){
            printf("Error in line %d : statement should be CONTINUE SEMICOLON\n",line_info[token_counter1-1]);
            return NULL;
        }
        root=statement_tree.set_head("continue_statement");
        readtoken();
        return root;
    }
    else if(!strcmp(w,"LP")){
        root=expression(RP);
        readtoken();
        return root;
    }
    else if(!strcmp(w,"ID")){
        root=expression(SEMICOLON);
        readtoken();
        return root;
    }
    else if(!(strcmp(w,"INT_LITERAL")&&strcmp(w,"FLOAT_LITERAL")&&strcmp(w,"CHAR_LITERAL"))){
        root=expression(SEMICOLON);
        readtoken();
        return root;
    }
    else if(!strcmp(w,"RBRACE")){
        readtoken();
        return NULL;
    }
    else{
        errors++;
        printf("Error in line %d : undefined token in the statement\n",line_info[token_counter1-1]);
        return NULL;
    }
}

//the function used to operate the expression, ended with endsym,which is semicolon or RP
//the first token of expression is already read when the function is called
//the subsequent token is not read when the function returns
tree<std::string>::iterator expression(int endsym){
    stack<std::string> operator_stack;
    stack< tree<std::string>::iterator > operand_stack;
    operator_stack.push("#");
    
    tree<std::string> tmp_tree;
    tree<std::string> tmp_node;
    tree<std::string>::iterator t1;
    tree<std::string>::iterator t2;

    tree<std::string>::iterator root;
    tree<std::string>::iterator p;
    
    int error=0;
    std::string t;

   
    while((strcmp(w,"#")||operator_stack.top().compare("#"))&&!error){
        if(!strcmp(w,"ID")||!strcmp(w,"INT_LITERAL")||!strcmp(w,"FLOAT_LITERAL")||!strcmp(w,"CHAR_LITERAL")){
            p=tmp_node.set_head("node");
            tmp_node.append_child(p,w);
            operand_stack.push(p);
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
                    //generate the tree   need to be modified
                    root=tmp_tree.set_head(t);                   
                    tmp_tree.append_child(root,t1);
                    tmp_tree.append_child(root,t2);
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
                        strcpy(w,"#");
                    }
                    else{
                        error=1;
                    }
            }
        }
        else if(!strcmp(w,EnumtoStr(endsym))){
            strcpy(w,"#"); 
        }
        else{
            error=1;
        }
        if(operand_stack.size()==1&&!operator_stack.top().compare("#")){
            return root;
        }
        else{
            printf("Error in line %d : expression should be expression SEMICOLON or expression RP\n",line_info[token_counter1-1]);
            return NULL;
        }  
    }
    return NULL;
}


//the function used to traverse and print the tree
void treePrint(){
    tree<std::string>::iterator it;
    int depth;
    for(it=program_tree.begin();it!=program_tree.end();++it){
        depth=program_tree.depth(it);
        printf("--%*s%s\n",depth*2,"",(*it).c_str());
    }
}


