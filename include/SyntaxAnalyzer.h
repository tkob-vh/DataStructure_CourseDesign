/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/

extern char w[20]; // store the token kind of contomparary token in SyntaxAnalyzer.c
extern bool error_flag; // once error_flag is 1, release the memory of AST .
extern char token_text[1000][20]; // store the token text such as identifier, literal, etc.
extern char t_kinds[1000][20]; // store the token kind such as ID, bool_LITERAL, etc.
extern int line_info[1000]; // store the line number of each token
extern size_t line_counter; // count the line number
extern size_t token_counter; // count the token number in LexicalAnalyzer.c
extern size_t token_counter1; // count the token number in SyntaxAnalyzer.c

bool program();
bool external_defination_list();
bool external_defination();
bool external_variable_defination();
bool function_defination();
bool type_specifier();
bool variable_list();
bool parameter();
bool function_name();
bool compound_statement();
bool formal_parameter_list();
bool formal_parameter();
bool local_variable_defination_list();
bool local_variable_defination();
bool statement_list();
bool statement();
bool expression_statement();
bool expression();
