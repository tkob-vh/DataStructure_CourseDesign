/********************************************************************
* FILE: GlobalVar.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/




// The token kind
typedef enum 
{
ERROR_TOKEN,
ID,
INT_LITERAL,
FLOAT_LITERAL,
CHAR_LITERAL,

INT,
FLOAT,
CHAR,
VOID,
IF,
ELSE,
WHILE,
RETURN,
FOR,
DO,
BREAK,
CONTINUE,
DEFINE,
INCLUDE,
SHARP,

LINECOMMENT,
BLOCKCOMMENT,

PLUS,
MINUS,
MULTIPLY,
DIVIDE,
MOD,


ASSIGN,
EQ,
NEQ,
GREATER,
LESS,
GREATER_EQ,
LESS_EQ,

LBRACE,
RBRACE,
LBRACKET,
RBRACKET,
LP,
RP,

AND,
OR,

SEMICOLON,
SINGLE_QUOTE,
DOUBLE_QUOTE,

END_OF_FILE,
}Token_kind;

Token_kind token_kind;


char TK[46][15]={"ERROR_TOKEN","ID","INT_LITERAL","FLOAT_LITERAL","CHAR_LITERAL","INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE","SHARP","LINECOMMENT","BLOCKCOMMENT","PLUS","MINUS","MULTIPLY","DIVIDE","MOD","ASSIGN","EQ","NEQ","GREATER","LESS","GREATER_EQ","LESS_EQ","LBRACE","RBRACE","LBRACKET","RBRACKET","LP","RP","AND","OR","SEMICOLON","SINGLE_QUOTE","DOUBLE_QUOTE","END_OF_FILE"};
