#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <glob.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX(x,y) ((x)<(y) ? (y) : (x))
#define MIN(x,y) ((x)<(y) ? (x) : (y))

#ifndef __GNUC__

#define __attribute__(x)
#endif

typedef struct Type Type;
typedef struct Node Node;
typedef struct Member Member;
typedef struct Relocation Relocation;
typedef struct Hideset Hideset;


typedef enum{
    TK_IDENT,
    TK_PUNCT,
    TK_KEYWORD,
    TK_STR,
    TK_NUM,
    TK_PP_NUM,
    TK_EOF,
} TokenKind;

typedef struct {
    char *name;
    int file_no;
    char *contents;
    char *display_name;
    int line_delta;
}File;


typedef struct Token Token;

struct Token{
    TokenKind kind;
    Token *next;
    int64_t val;
    long double fval;
    char *loc;
    int len;
    Type *ty;
    char *str;

    File *file;
    char *filename;
    int line_no;
    int line_delta;
    bool at_bol;
    bool has_space;
    Hideset *hideset;
    Token *origin;

};

noreturn void error(char *fmt, ...) __attribute__((format(printf,1,2)));
noreturn void error_at(char *loc,char *fmt, ...) __attribute__((format(printf,2,3)));
noreturn void error_tok(Token *tok,char *fmt) __attribute__((format(printf,2,3)));

void ward_tok(Token *tok,char *fmt, ...) __attribute__((format(printf,2,3)));
bool equal(Token *tok,char *op);
Token *skip(Token *tok,char *op);
bool consume(Token **rest,Token *tok,char *str);
void convert_pp_tokens(Token *tok);
File **get_input_files(void);
File *new_file(char *name,int file_no,char *contents);
Token *tokenize_string_literal(Token *tok,Type *basety);
Token *tokenize(File *file);
Token *tokenize_file(char *filename);