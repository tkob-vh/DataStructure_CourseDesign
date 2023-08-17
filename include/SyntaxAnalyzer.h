/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/

extern char w[20]; // store the token kind of contomparary token in SyntaxAnalyzer.c
extern bool error_flag; // once error_flag is 1, release the memory of AST .
extern char token_text[1000][20]; // store the token text such as identifier, literal, etc.
extern char t_kinds[1000][20]; // store the token kind such as ID, int_LITERAL, etc.
extern int line_info[1000]; // store the line number of each token
extern char Capital_Keywords[14][10]; // store the main keywords
extern size_t line_counter; // count the line number
extern size_t token_counter; // count the token number in LexicalAnalyzer.c
extern size_t token_counter1; // count the token number in SyntaxAnalyzer.c
#include<tree.hh>

tree<std::string>::iterator program();
tree<std::string>::iterator external_defination_list();
//tree<std::string>::iterator external_defination();
tree<std::string>::iterator external_variable_defination();
tree<std::string>::iterator function_defination();
tree<std::string>::iterator type_specifier();
tree<std::string>::iterator variable_list();
tree<std::string>::iterator parameter();
tree<std::string>::iterator function_name();
tree<std::string>::iterator compound_statement();
tree<std::string>::iterator formal_parameter_list();
tree<std::string>::iterator formal_parameter();
tree<std::string>::iterator local_variable_defination_list();
tree<std::string>::iterator local_variable_defination();
tree<std::string>::iterator statement_list();
tree<std::string>::iterator statement();
tree<std::string>::iterator expression_statement();
tree<std::string>::iterator expression();
