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

#define unreachable(){
    error("internal error at %s:%d",__FILE__,__LINE__);
}


char *search_include_paths(char *filename);
void init_macros(void);
void define_macro(char *name,char *buf);
void underf_macro(char *name);
Token *preprocess(Token *tok);


typedef struct Obj Obj;

struct Obj{
    Obj *next;
    char *name;
    Type *ty;
    Token *tok;
    bool is_local;
    int align;
    int offset;
    bool is_function;
    bool is_tls;
    char *init_data;
    Relocation *rel;
    bool is_inline;
    Obj *params;
    Node *body;
    Obj *locals;
    Obj *va_area;
    Obj *alloca_bottom;
    int stack_size;
    bool is_live;
    bool is_root;
    StringArray refs;
};

typedef struct Relocation Relocation;

struct Relocation{
    Relocation *next;
    int offset;
    char **label;
    int addend;
};

typedef enum {
  ND_NULL_EXPR, // Do nothing
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_NEG,       // unary -
  ND_MOD,       // %
  ND_BITAND,    // &
  ND_BITOR,     // |
  ND_BITXOR,    // ^
  ND_SHL,       // <<
  ND_SHR,       // >>
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_ASSIGN,    // =
  ND_COND,      // ?:
  ND_COMMA,     // ,
  ND_MEMBER,    // . (struct member access)
  ND_ADDR,      // unary &
  ND_DEREF,     // unary *
  ND_NOT,       // !
  ND_BITNOT,    // ~
  ND_LOGAND,    // &&
  ND_LOGOR,     // ||
  ND_RETURN,    // "return"
  ND_IF,        // "if"
  ND_FOR,       // "for" or "while"
  ND_DO,        // "do"
  ND_SWITCH,    // "switch"
  ND_CASE,      // "case"
  ND_BLOCK,     // { ... }
  ND_GOTO,      // "goto"
  ND_GOTO_EXPR, // "goto" labels-as-values
  ND_LABEL,     // Labeled statement
  ND_LABEL_VAL, // [GNU] Labels-as-values
  ND_FUNCALL,   // Function call
  ND_EXPR_STMT, // Expression statement
  ND_STMT_EXPR, // Statement expression
  ND_VAR,       // Variable
  ND_VLA_PTR,   // VLA designator
  ND_NUM,       // Integer
  ND_CAST,      // Type cast
  ND_MEMZERO,   // Zero-clear a stack variable
  ND_ASM,       // "asm"
  ND_CAS,       // Atomic compare-and-swap
  ND_EXCH,      // Atomic exchange
} NodeKind;