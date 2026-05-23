#include "chibicc.h"

static File *current_file;

static File **input_files;

static bool at_bol;

static bool has_space;
void error(char *fmt, ...){
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}


static void verror_at(char *filename,char *input,int line_no,char *loc,char *fmt,va_list ap){
    char *line=loc;
    while(input<line && line[-1]!='\n'){
        line--;
    }
    char *end=loc;
    while(*end && *end!='\n'){
        end++;
    int indent = fprintf(stderr, "%s:%d: ", filename, line_no);
    fprintf(stderr, "%.*s\n", (int)(end - line), line);
    int pos=display_width(line,loc-line)+indent;
    fprintf(stderr, "%*s", pos, ""); // print pos spaces.
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

void error_tok(Token *tok, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(tok->file->name, tok->file->contents, tok->line_no, tok->loc, fmt, ap);
  exit(1);
}

void error_tok(Token *tok,)
