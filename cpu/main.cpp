#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "assembler.h"
#include "translate.h"
#include "parse.h"
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

String readFile(char *filename)
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
int main(int argc, char *argv[])
{
    const char* s = "hello %d sailor\n";
    int g = strlen(s);
    Assembler a = allocateSomeExecutablePages();

//    Label label = a.newLabel();

//    a.call(label);
//    a.ret();
//    a.bind(label);
//    a.mov(rdx, g);
//    a.mov(rsi, (int64_t)s);
//    a.mov(rdi, 0);
//    a.mov(rax, 1);
//    a.syscall();


//    a.mov(rdi, (int64_t)s);
//    a.mov(rsi, 1337);
//    a.mov(rax, (int64_t)(printf));
//    a.call(rax);



//    a.ret();

    ::String thing = readFile("program");
    ParseRes<AddExp> ast = addExp(&thing);
    if(ast.type == error) {
        printf("%.*s", ast.error.size, ast.error.content);
        return 1;
    }
    print(ast.parsed);
    printf("\n\n\n");
    translate(&a, &ast.parsed);
    Func fn;
    fn = (Func)(a.mem);
    int result = fn();                      // Execute the generated code.
    printf("result: %d\n", result);                 // Print the resulting "1".

    return 0;
}
