﻿#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <inttypes.h>
enum register64 {
    rax,
    rcx,
    rdx,
    rbx,

    rsp,
    rbp,

    rsi,
    rdi,
//    rip, call все делает за меня
};
enum : char {
    rex = 0x40,
    rexw = 0x08,
    rexr = 0x01,
};

struct Label
{
    int id;
};

struct Assembler
{
    char* mem = nullptr;
    int size = 0;
    int position = 0;
    int nextLabel = 0;
    struct LabelLinkListNode
    {
        LabelLinkListNode* tail = 0;
        bool relative;
        int place = 0;
    };
    struct LabelTableEntry {
            LabelLinkListNode* links = 0;
    } labelTable[255] = {0};


    void dump();

    Label newLabel() {
        Label res{nextLabel};
        nextLabel++;
        return res;
    }

    void call(Label l) {
        mem[position] = 0xe8;
        LabelLinkListNode* head = new LabelLinkListNode;
        head->place = position+1;
        head->relative = true;
        head->tail = labelTable[l.id].links;
        labelTable[l.id].links = head;
        position+=5;
        dump();
    }
    void call(register64 add) {
        mem[position] = 0xff;
        mem[position+1] = (0b11 << 6/*mod*/) |
                (0x02 << 3) | add;

        position+=2;
        dump();
    }
    void ret() {
        mem[position] = 0xc3;
        position++;
        dump();
    }
    void bind(Label l) {
        LabelLinkListNode* llln = labelTable[l.id].links;
        while (llln) {
            if(llln->relative) {
                int32_t theNumber;
                theNumber = position - (llln->place + 4); // +4 is becuse it needs to be relative to the next instruction
                *(mem + llln->place) = theNumber & 0xff;
                *(mem + llln->place+1) = (theNumber>>8) & 0xff;
                *(mem + llln->place+2) = (theNumber>>16) & 0xff;
                *(mem + llln->place+3) = (theNumber>>24) & 0xff;
            } else {
                int64_t theNumber;
                theNumber = (int64_t)(mem + position);
                *((int64_t*)(mem + llln->place)) = theNumber;
            }
            llln = llln->tail;
        }
        dump();
    }
    void mov(register64 dest, int64_t value) {
        mem[position] = 0x48;
        mem[position+1] = 0xb8 + dest;
        *((int64_t*)(mem+position+2)) = value;
        position += 10;
        dump();
    }
    void mov(register64 dest, register64 src) {
        mem[position] = rex | rexw;
        mem[position+1] = 0x89;
        mem[position+2] = (0b11 << 6) | (src << 3) | dest;
        position += 3;
        dump();
    }
    void mov(register64 dest, Label l) {
        mem[position] = 0x48;
        mem[position+1] = 0xb8 + dest;

        LabelLinkListNode* head = new LabelLinkListNode;
        head->place = position+2;
        head->relative = false;
        head->tail = labelTable[l.id].links;
        labelTable[l.id].links = head;
        position += 10;
        dump();
    }
    void syscall() {
        mem[position] = 0x0f;
        mem[position+1] = 0x05;
        position += 2;
        dump();
    }
    void push(register64 src) {
        mem[position] = 0x50+src;
        position++;
        dump();
    }
    void pushRbpPlusOffset(int offset) {
        // a. k. a. push stack argument
        mem[position] = 0xff;
        mem[position+1] = 0x75;
        mem[position+2] = offset;
        position+=3;
        dump();
    }
    void pop(register64 dest) {
        mem[position] = 0x58+dest;
        position++;
        dump();
    }
    void add(register64 dest, register64 addee) {
        mem[position] = rex | rexw;
        mem[position+1] = 0x01;
        mem[position+2] = (0b11 << 6/*mod*/) | (addee << 3) | dest;

        position+=3;
        dump();
    }
    void add(register64 dest, int8_t addee) {
        mem[position] = rex | rexw;
        mem[position+1] = 0x83; // хуй знает правильно ли это, я хз
        mem[position+2] = (0b11 << 6/*mod*/) | (0x000 << 3) | dest;
        mem[position+3] = addee;

        position+=4;
        dump();
    }
    void imul(register64 dest, register64 operand) {
        mem[position] = rex | rexw;
        mem[position+1] = 0x0f;
        mem[position+2] = 0xaf;
        mem[position+3] = (0b11 << 6/*mod*/) | (dest << 3) | operand;

        position+=4;
        dump();
    }
    void idiv(register64 operand) {
        mem[position] = rex | rexw;
        mem[position+1] = 0xf7;
        mem[position+2] = (0b11 << 6/*mod*/) | (0x07 << 3) | operand;

        position+=3;
        dump();
    }
    void cqo() {
        mem[position] = rex | rexw;
        mem[position+1] = 0x99;
        position+=2;
        dump();
    }
    void sub(register64 dest, register64 addee) {
        mem[position] = rex | rexw;
        mem[position+1] = 0x29;
        mem[position+2] = (0b11 << 6/*mod*/) | (addee << 3) | dest;

        position+=3;
        dump();
    }

};

Assembler allocateSomeExecutablePages();

#endif // ASSEMBLER_H
