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

void emitItoa(Assembler *a)
{
    a->push(rax);
    a->push(rbx);
    a->push(rcx);
    a->push(rdx);
    a->push(rdi);

    a->mov(rbx, 10); // base of the decimal system
    a->mov(rcx, 0);  // number of digits generated

    Label nxdiv = a->newLabel();
    Label neg = a->newLabel();
    a->mov(rsi, nxdiv);
    a->mov(r8, neg);
    a->cmp(rax, 0);
    a->cmovs(rsi, r8);
    a->jmp(rsi);

    a->bind(nxdiv);
    a->mov(rdx, 0);  // RAX extended to (RDX,RAX)
    a->div(rbx);    // divide by the number-base
    a->push(rdx);    // save remainder on the stack
    a->inc(rcx);    // and count this remainder
    a->cmp(rax, 0);  // was the quotient zero?
    a->jne(nxdiv);   // no, do another division
    Label nxdgt = a->newLabel():
    a->bind(nxdgt);
    a->pop(rdx);    // else pop recent remainder
    a->add(dl, '0'); // and convert to a numeral
    a->movToMem(rdi, dl); // store to memory-buffer
    a->inc(rdi);    // and advance buffer-pointer
    a->loop(nxdgt);   // again for other remainders

    a->pop(rdi);    // recover saved registers
    a->pop(rdx);
    a->pop(rcx);
    a->pop(rbx);
    a->pop(rax);
    a->ret()     // and return to the caller
}
