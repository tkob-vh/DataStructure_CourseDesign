/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/




// The token kind
enum token_kind
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
};



//global variables
char token_text[100][10]; // store the token text such as identifier, literal, etc.
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number
char Keywords[14][10]={"int","float","char","void","if","else","while","return","for","do","break","continue","define","include"};//store the main keywords
int w; //store the token kind of contomparary token in SyntaxAnalyzer.c