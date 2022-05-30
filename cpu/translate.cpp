#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "translate.h"
#include "parse.h"
#include "assembler.h"
#include <queue>

int extractItoa(char* loc) {
    char path[4096] = {0};
    strcat(path, MY_PATH);
    strcat(path, "/exe");
    int exe = open(path, O_RDONLY);
    lseek(exe, 0x00000000004000f3, SEEK_SET);
    int len = 0x40012b - 0x4000f3;
    read(exe, loc, len);
    return len;
}

struct QueuedLambda {
    struct Lambda* lam;
    Label lab;
};
std::queue<struct QueuedLambda> queue;
void translateAdd(Assembler* a, AddExp *exp);

void translatePrim(Assembler* a, Prim *exp) {
    if(exp->type == Literal) {
        a->mov(rax, (int64_t)(exp->lit));
        a->push(rax);
    }
    if(exp->type == Variable) {
        a->pushRbpPlusOffset((exp->var.index+2)*8);
    }
    if(exp->type == AddExpParen) {
        translateAdd(a, exp->addExp);
    }

    if(exp->type == Lambda) {
        Label l = a->newLabel();
        queue.push(QueuedLambda{&exp->lam, l});
//        CodeHolder::n
        a->mov(rax, l);
        a->push(rax);
    }
}

void translateApp(Assembler* a, Application *exp) {
    bool actuallyApplication = exp->applicand != 0;
    translatePrim(a, &exp->parameter);
//    int arity = translatePrim(a, exp->parameter);
    int paramNumber = 0;
    while(exp->applicand) {
        translatePrim(a, &exp->applicand->parameter);
        paramNumber++;
        exp = exp->applicand;
    }
    if(actuallyApplication) {
        a->pop(rax);
        a->call(rax);
        a->add(rsp, int8_t(0x8*paramNumber));
//        a->pop(rdx);
        a->push(rax);
    }
}

void translateMul(Assembler* a, MulExp *exp) {
    translateApp(a, &exp->left);
    while(exp->r) {
        translateApp(a, &exp->r->right.left);
        a->pop(rcx);
        a->pop(rax);
        if(exp->r->op == '*') {
            a->imul(rax, rcx);
        } else {
            a->cqo();
            a->idiv(rcx);
        }
        a->push(rax);
        exp = &exp->r->right;
    }
}

void translateAdd(Assembler* a, AddExp *exp) {
    translateMul(a, &exp->left);
    while(exp->r) {
        translateMul(a, &exp->r->right.left);
        a->pop(rcx);
        a->pop(rax);
        if(exp->r->op == '+') {
            a->add(rax, rcx);
        } else {
            a->sub(rax, rcx);
        }
        a->push(rax);
        exp = &exp->r->right;
    }
}
extern int itoap;
void translate(Assembler *a, AddExp *exp)
{

    translateAdd(a, exp);
    a->push(rax);

    char* strin = "Hello, world!\n";
    Label string = a->newLabel();


    Label itoa = a->newLabel();
    a->mov(rdi, string);
    a->pop(rax);

    a->mov(rbx, itoa);
    a->call(rbx);

    a->mov(rax, 1);
    a->mov(rdi, 1);
    a->mov(rsi, string);
    a->mov(rdx, strlen(strin));
    a->syscall();

//    a->mo1v(rdi, (int64_t)s);
//    a->pop(rsi);
//    a->mov(rax, 0);
//    a->mov(rbx, (int64_t)(printf));
//    a->call(rbx);



#define EXIT 60
    a->mov(rax, EXIT);
//    a->pop(rdi);
    a->mov(rdi, 0);
    a->syscall();

    a->bind(string);
    a->position+=20;//bytes(strin, strlen(strin));

    a->bind(itoa);
    itoap = a->position;
    a->position += extractItoa(a->mem+a->position);
    while (!queue.empty()) {
        QueuedLambda ql = queue.front();
        queue.pop();
        a->bind(ql.lab);

        a->push(rbp);
        a->mov(rbp, rsp);

        translateAdd(a, ql.lam->exp);
        a->pop(rax);

        a->pop(rbp);

        a->ret();
    }
}

//void translateLambda (asmjit::x86::Assembler* a, Lambda* lambda) {

//}
