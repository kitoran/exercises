#include <stdio.h>
#include "translate.h"
#include "parse.h"
#include "assembler.h"
#include <queue>

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
    translatePrim(a, &exp->parameter);
    bool actuallyApplication = exp->applicand != 0;
//    int arity = translatePrim(a, exp->parameter);
//    int paramNumber = 0;
    while(exp->applicand) {
        translatePrim(a, &exp->applicand->parameter);
//        paramNumber++;
        exp = exp->applicand;
    }
    if(actuallyApplication) {
        a->pop(rax);
        a->call(rax);
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

void translate(Assembler *a, AddExp *exp)
{
    const char* s = "%d\n";
    translateAdd(a, exp);

    a->mov(rdi, (int64_t)s);
    a->pop(rsi);
    a->mov(rax, 0);
    a->mov(rbx, (int64_t)(printf));
    a->call(rbx);


    a->mov(rax, 5255);
    a->ret();

    while (!queue.empty()) {
        QueuedLambda ql = queue.back();
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
