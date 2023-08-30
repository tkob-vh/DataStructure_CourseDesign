/********************************************************************
* FILE: Scanner.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/
#pragma once
#include "tree.hh"
#include "var.hh"


extern char w[20]; // store the token kind of contomparary token in Parser.c
extern bool error_flag; // once error_flag is 1, release the memory of AST .
extern char token_text[1000][20]; // store the token text such as identifier, literal, etc.
extern char t_kinds[1000][20]; // store the token kind such as ID, int_LITERAL, etc.
extern char TK[51][15];
extern int line_info[1000]; // store the line number of each token
extern char Capital_Keywords[16][10]; // store the main keywords
extern size_t line_counter; // count the line number
extern size_t token_counter; // count the token number in Scanner.c
extern size_t token_counter1; // count the token number in Parser.c
extern char temp_kind[20];
extern char temp_text[20];
extern int errors;

extern char op_table[17][17];
extern char table_arr[17][15];
extern char function_list[100][20];
extern int function_counter;


extern tree<std::string> program_tree;
extern tree<std::string>::iterator program_root;

extern tree<std::string> preprocessing_list_tree;
extern tree<std::string> external_defination_list_tree;








extern tree<std::string> empty_tree;

bool LookupCapitalKeywords(char* w);
void readtoken();
bool isOperator(char* w);
bool isFunction(char* w);
Token_kind StrtoEnum(char * str);
int getTableNum(const char * w);
char getOperatorPriority(const char* w1,char* w2);

tree<std::string>::iterator program();
tree<std::string> preprocessing_list();
tree<std::string> preprocessing();
tree<std::string> external_defination_list();
tree<std::string> external_defination();
tree<std::string> external_variable_defination();
tree<std::string> function_defination();
tree<std::string> formal_parameter_list();
tree<std::string> formal_parameter();
tree<std::string> compound_statement();
tree<std::string> local_variable_defination_list();
tree<std::string> local_variable_defination();
tree<std::string> statement_list();
tree<std::string> statement();
tree<std::string> expression(int endsym);
void treePrint();
void printTree();
