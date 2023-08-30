/********************************************************************
* FILE:IRGenerator.h 
* DESCRIPTION:
* AUTHOR: yyx
********************************************************************/
#pragma once
#include"tree.hh"

extern tree<std::string> program_tree;
extern tree<std::string>::iterator program_root;
extern tree<std::string>::iterator it;


extern int brace_counter;
extern char token_text[1000][20];

bool program_code();
bool preprocessing_code(FILE *fp);
bool external_defination_list_code(FILE *fp);