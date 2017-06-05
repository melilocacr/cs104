 /* $Id: scanner.l,v 1.0 2017-05-18 5:18:16 - - $ */

%{

#include "lyutils.h"

#define YY_USER_ACTION  { lexer::advance(); }

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn

LETTER          [A-Za-z_]
DIGIT           [0-9]
INTCON          ({DIGIT}+)
CHARCON         ('([^\\'\n]|\\[\\'"0nt])')
NOTCHARCON      ('([^\\'\n]|\\[\\'"0nt]?)[^']?)
STRINGCON       (\"([^\\"\n]|\\[\\'"0nt])*\")
NOTSTRINGCON    (\"([^\\"\n]|\\[\\'"0nt])*[^\"]?)
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
NOTIDENT        ({DIGIT}+{LETTER}({LETTER}|{DIGIT})*)

%%

"#".*           { lexer::include(); }
[ \t]+          { }
\n              { lexer::newline(); }

"void"          { return yylval_token (TOK_VOID); }
"char"          { return yylval_token (TOK_CHAR); }
"int"           { return yylval_token (TOK_INT); }
"string"        { return yylval_token (TOK_STRING); }
"if"            { return yylval_token (TOK_IF); }
"else"          { return yylval_token (TOK_ELSE); }
"while"         { return yylval_token (TOK_WHILE); }
"return"        { return yylval_token (TOK_RETURN); }
"struct"        { return yylval_token (TOK_STRUCT); }
"null"          { return yylval_token (TOK_NULL); }
"new"           { return yylval_token (TOK_NEW); }
"ord"           { return yylval_token (TOK_ORD); }
"chr"           { return yylval_token (TOK_CHR); }
"[]"            { return yylval_token (TOK_ARRAY); }

{INTCON}        { return yylval_token (TOK_INTCON); }
{IDENT}         { return yylval_token (TOK_IDENT); }
{CHARCON}       { return yylval_token (TOK_CHARCON); }
{STRINGCON}     { return yylval_token (TOK_STRINGCON); }

"+"             { return yylval_token ('+'); }
"-"             { return yylval_token ('-'); }
"*"             { return yylval_token ('*'); }
"/"             { return yylval_token ('/'); }
"%"             { return yylval_token ('%'); }
"!"             { return yylval_token ('!'); }
"="             { return yylval_token (TOK_EQ); }
","             { return yylval_token (','); }
"."             { return yylval_token ('.'); }
"^"             { return yylval_token ('^'); }
"("             { return yylval_token ('('); }
")"             { return yylval_token (')'); }
"["             { return yylval_token ('['); }
"]"             { return yylval_token (']'); }
"{"             { return yylval_token ('{'); }
"}"             { return yylval_token ('}'); }
";"             { return yylval_token (';'); }

"="             { return yylval_token (TOK_EQ); }
"=="            { return yylval_token (TOK_EQEQ); }
"!="            { return yylval_token (TOK_NE); }
"<"             { return yylval_token (TOK_LT); }
"<="            { return yylval_token (TOK_LE); }
">"             { return yylval_token (TOK_GT); }
">="            { return yylval_token (TOK_GE); }


{NOTIDENT}      { lexer::badtoken(yytext); }
{NOTCHARCON}    { lexer::badtoken(yytext); }
{NOTSTRINGCON}  { lexer::badtoken(yytext); }
.               { lexer::badchar(*yytext); }


%%
