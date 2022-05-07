#ifndef PARSE_H
#define PARSE_H
#include <assert.h>

enum PrimType {
    Lambda,
    AddExpParen,
    Variable,
    Literal
};
namespace ju {
struct String {
    int size; //= 0;
    char* content; //= 0;
    void advance(int i) {
        assert(size >= i);
        size -= i;
        content += i;
    }
//    String() = default;
};
}
using namespace ju;

struct AddExp;
struct VarList {
    String var;
    VarList* rest;
};
struct Lambda {
    VarList vars;
    AddExp* exp;
};
enum MulOp {
    mul = '*',
    divi = '/',
};
struct Var
{
    String name;
    int index;
};
struct Prim
{
    PrimType type;
    union {
        struct Lambda lam;
        struct AddExp* addExp;
        Var var;
        int lit;
    };
};
struct Application {
    Application* applicand;
    Prim parameter;
};
struct MulExpR;
struct MulExp {
    Application left;
    struct MulExpR* r;
};
struct MulExpR {
    MulOp op;
    MulExp right;
};

enum AddOp : char {
    plus = '+',
    minus = '-',
};
struct AddExpR;
struct AddExp {
    MulExp left;
    AddExpR* r;
};
struct AddExpR {
    AddOp op;
    AddExp right;
};
enum ParseResType {
    ok,
    error
};
template <typename T>
struct ParseRes {
    ParseResType type;
    union {
        T parsed;
        ju::String error;
    };
};

ParseRes<AddExp> addExp(String* thing);
inline struct Lambda* lambdaStack[200];
inline int lambdasNum = 0;

#endif // PARSE_H
