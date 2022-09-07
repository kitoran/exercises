#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "assembler.h"
#include "translate.h"
#include "parse.h"


#include <readline/readline.h>
//-fuse-ld
void print(const AddExp& ae);
void print(const Prim& p) {
    if(p.type == Lambda) {
        printf("\\");
        printf("%.*s", p.lam.vars.var.size,
               p.lam.vars.var.content);
        const VarList* vl = (p.lam.vars.rest);
        while(vl) {
            printf(" %.*s", vl->var.size, vl->var.content);
            vl = vl->rest;
        }
        printf(".");
        print(*(p.lam.exp));
    }
    if(p.type == AddExpParen) {
        printf("(");
        print(*(p.addExp));
        printf(")");
    }
    if(p.type == Variable) {
        printf("%.*s%d", p.var.name.size,
               p.var.name.content, p.var.index);
    }
    if(p.type == Literal) {
        printf("%d", p.lit);
    }
}
void print(const Application& a) {
    if(a.applicand) {
        print(*a.applicand);
        printf(" ");
    }
    print(a.parameter);
}
void print(const MulExp& ae) {
    print(ae.left);
    if(ae.r) {
        printf("%c", ae.r->op);
        print(ae.r->right);
    }
}
void print(const AddExp& ae) {
    print(ae.left);
    if(ae.r) {
        printf("%c", ae.r->op);
        print(ae.r->right);
    }
}

String readFile(const char *filename)
{   //gets the PATH of the txt file
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen(filename, "r");
    if(input_file == NULL)
    {
         return {0, NULL};
    }
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = (char*)malloc(input_file_size * (sizeof(char)));
    fread(file_contents, input_file_size, 1, input_file);
    fclose(input_file);
    String res;
    res.size = input_file_size;
    res.content = file_contents;
    return res;
}
typedef int (*Func)(void);
void mysighandler(int s)
{
#define pascalstring(a) a, sizeof(a)-1
    write(1, sys_siglist[s], strlen(sys_siglist[s]));
    abort();
}
void printHelp() {
    printf("Hello, this is a compiler.\n"
           "start with -i for repl "
           "or with a file as an argument\n"
           "(there is also a secret option -d)\n");
}
void repl(Assembler a)
{
    printf("Welcome to the compiler of untyped supercombinators\n");
    char* line = readline("> ");
    while(line) {
        size_t size = strlen(line);
        assert(size < 1<<16);
        String exp = {(int)size, line};
        ParseRes<AddExp> ast = addExp(&exp);

        if(ast.type == error) {
            printf("%.*s\n", ast.error.size, ast.error.content);
        } else {
            translate(&a, &ast.parsed);
            Func fn;
            fn = (Func)(a.mem);
            int result = fn();                      // Execute the generated code.
            printf("result: %d\n", result);                 // Print the resulting "1".
        }
        free(line);
        line = readline("> ");
    }
}
int itoap;
void compileAndRun(Assembler a, const char* path) {
    String thing = readFile(path);
    if(thing.content == NULL) {
        printf("path not valid or file doesn't exist\n");
        return;
    }
    printf("\"%.*s\"\n", thing.size, thing.content);
    ParseRes<AddExp> ast = addExp(&thing);
//    String errorMe = ast.error;
    if(ast.type == error) {
        printf("%.*s\n", ast.error.size, ast.error.content);
        return;
    }
    print(ast.parsed);
    printf("\n\n\n");
    translate(&a, &ast.parsed);
    Func fn;
    fn = (Func)(a.mem);
    int result = fn();                      // Execute the generated code.
    printf("result: %d\n", result);                 // Print the resulting "1".
}
int main(int argc, char *argv[])
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = mysighandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGSEGV, &sigIntHandler, NULL);
    sigaction(SIGFPE, &sigIntHandler, NULL);

    Assembler a = allocateSomeExecutablePages();

    if(argc == 2 && !memcmp(argv[1], "-i", strlen(argv[1]))) {
        repl(a);
        return 0;
    }
    if(argc == 2 && argv[1][0] != '-') {
        compileAndRun(a, argv[1]);
        return 0;
    }
    if(argc == 2 && !memcmp(argv[1], "-d", strlen(argv[1]))) {
        compileAndRun(a, "program");
        repl(a);
        return 0;
    }

    printHelp();
    return 1;
}
