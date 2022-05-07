#include "parse.h"
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <assert.h>

const int em = 255;
char errorMessage[em];
//String boundVars[200];
//int boundVarsNum = 0;


#define ERROR(...) res.type = error; \
    String ems; \
    ems.size = snprintf(errorMessage, em, __VA_ARGS__); \
    ems.content = errorMessage; \
    res.error = ems; \
    return res;

#define UNEXPECTED_END if(thing->size == 0) { \
    ERROR("unexpected end of file, additional info: %s %d",  __FILE__, __LINE__); \
}

#define PROMOTE_ERROR(parseRes) if((parseRes).type == error) { \
    res.type = error; \
    res.error = (parseRes).error; \
    return res; \
}

//void pushVar(const String& var) {
//    boundVars[boundVarsNum] = var;
//    boundVarsNum++;
//}
//void popVar() {
//    boundVarsNum--;
//}
int findVarInCurrentLambda(const String& var) {
    assert(lambdasNum >= 0);
    if(lambdasNum == 0) {
        return -1;
    }
    VarList* t = &lambdaStack[lambdasNum-1]->vars;
    int r = 0;
    do {
        if(var.size == t->var.size &&
                !memcmp(var.content, t->var.content, var.size)) {
            return r;
        }
        t = t->rest;
        r++;
    } while(t != 0);
    return -1;
}
ParseRes<String> var (String* thing) {
    ParseRes<String> res;
    UNEXPECTED_END
    if(!isalpha(thing->content[0])) {
        ERROR("unexpected %c, expected alphaetic character", thing->content[0])
    }
    String name; name.content = thing->content;
    int size = 0;
    while(thing->size != 0 && isalnum(thing->content[0])) {
        size++;
        thing->advance(1);
    }
    name.size = size;

    res.type = ok;
    res.parsed = name;
    return res;
}
void skipWhitespase(String* thing) {
    while(thing->size > 0 && isspace(thing->content[0])) {
        thing->advance(1);
    }
}
ParseRes<AddExp> addExp(String* );
ParseRes<Prim> prim(String* thing) {
    ParseRes<Prim> res;

    Prim p;
    skipWhitespase(thing);
    UNEXPECTED_END
    char sym = thing->content[0];
    if(sym == '\\') {
        p.type = Lambda;
        thing->advance(1);
        struct VarList varList;
        struct VarList* end = &varList;
        skipWhitespase(thing);
        int arity = 1;
        {
            ParseRes<String> varName = var(thing);
            PROMOTE_ERROR(varName)

            skipWhitespase(thing);
            varList.var = varName.parsed;
//            pushVar(varList.var);
            varList.rest = 0;
        }
        UNEXPECTED_END
        while (isalpha(thing->content[0])) {
            arity++;
            ParseRes<String> varName = var(thing);
            PROMOTE_ERROR(varName)
            end->rest = new VarList;
            end = end->rest;
            end->var = varName.parsed;
//            pushVar(end->var);
            end->rest = 0;
            skipWhitespase(thing);

            UNEXPECTED_END
        }
        if(thing->content[0] != '.') {
            ERROR("unexpected %c, expected '.'", thing->content[0])
        }
        thing->advance(1);

//        struct Lambda lam;
        p.lam.vars = varList;
        // buffer overflow
        lambdaStack[lambdasNum] = &p.lam;//&p.lam;
        lambdasNum++;
        ParseRes<AddExp> ae = addExp(thing);
        PROMOTE_ERROR(ae)
        lambdasNum--;
        p.lam.exp = new AddExp(ae.parsed);
//        p.lam = lam;
        res.type = ok;
        res.parsed = p;
//        for(int i = 0; i < arity; i++) {
//            popVar();
//        }
        return res;
    }
    if(sym == '(') {
        thing->advance(1);
        skipWhitespase(thing);
//        if(thing->content[0] == '\\') {
//            // эта ветка просто чтобы дерево получше выглядело
//            ParseRes<Prim> lam = prim(thing);
//            PROMOTE_ERROR(lam)
//        } else {
            p.type = AddExpParen;
            ParseRes<AddExp> ae = addExp(thing);
            PROMOTE_ERROR(ae)
            p.addExp = new AddExp(ae.parsed);
            skipWhitespase(thing);
//        }
        UNEXPECTED_END
        assert(thing->content[0] == ')');
        thing->advance(1);
        res.type = ok;
        res.parsed = p;
        return res;
    }
    if(isalpha(sym)) {
        p.type = Variable;
        ParseRes<String> varName = var(thing);
        PROMOTE_ERROR(varName)
        String name = varName.parsed;

        int index = findVarInCurrentLambda(name);
        if(index == -1) {
            ERROR("not a supercombinator,"
                  "free variable %.*s", name.size,
                  name.content)
        }
        p.var = Var{ .name = name, .index = index };
        res.type = ok;
        res.parsed = p;
        return res;
    }
    if(isdigit(sym)) {
        p.type = Literal;
        int theNumber = 0;
        while(thing->size != 0 && isdigit(thing->content[0])) {
            theNumber = theNumber*10 + thing->content[0] - '0';
            thing->advance(1);
        }
        p.lit = theNumber;
        res.type = ok;
        res.parsed = p;
        return res;
    }
    ERROR("unexpected %c, expected primary expression",
          thing->content[0])
}

ParseRes<Application> application(String* thing) {
    ParseRes<Application> res;

    Application app;
    app.applicand = 0;
    while(true) {
        ParseRes<Prim> left = prim(thing);
        PROMOTE_ERROR(left)
        app.parameter = left.parsed;
        skipWhitespase(thing);
        bool end;
        if(thing->size == 0) {
            end = true;
        } else {
            char next = thing->content[0];
            if(next == '\\' || isalnum(next) || next == '(') {
                end = false;
            } else {
                end = true;
            }
        }
        if(end) {
            res.type = ok;
            res.parsed = app;
            return res;
        }
        app.applicand = new Application(app);
    }
}


ParseRes<MulExp> mulExp(String* thing) {
    ParseRes<MulExp> res;
    ParseRes<Application> left = application(thing);
    PROMOTE_ERROR(left)
    MulExp mulexp;
    mulexp.left = left.parsed;
    mulexp.r = 0;
//    cont:
    skipWhitespase(thing);
    if(thing->size == 0) {
        res.type = ok;
        res.parsed = mulexp;
        return res;
    }
    char op = thing->content[0];
    if(op == '*' || op == '/') {
        thing->advance(1);
        ParseRes<MulExp> right = mulExp(thing);
        if(right.type == error) {
            res.type = error;
            res.error = right.error;
            return res;
        }
        MulExpR r;
        r.op = op=='*'?mul:divi;
        r.right = right.parsed;
        mulexp.r = new MulExpR(r);
        res.type = ok;
        res.parsed = mulexp;
        return res;
    }
    res.type = ok;
    res.parsed = mulexp;
    return res;
//    ERROR("unexpected %c, expected multiplicative expression", thing->content[0])
}

ParseRes<AddExp> addExp(String* thing) {
    ParseRes<AddExp> res;
    ParseRes<MulExp> left = mulExp(thing);
    PROMOTE_ERROR(left)
    AddExp addexp;
    addexp.left = left.parsed;
    addexp.r = 0;
//    cont:
    skipWhitespase(thing);
    if(thing->size == 0 || thing->content[0] == ')') {
        res.type = ok;
        res.parsed = addexp;
        return res;
    }
    char op = thing->content[0];
    if(op == '+' || op == '-') {
        thing->advance(1);
        ParseRes<AddExp> right = addExp(thing);
        if(right.type == error) {
            res.type = error;
            res.error = right.error;
            return res;
        }
        AddExpR r;
        r.op = op=='+'?plus:minus;
        r.right = right.parsed;
        addexp.r = new AddExpR(r);
        res.type = ok;
        res.parsed = addexp;
        return res;
    }
    ERROR("unexpected %c, expected additive expression", thing->content[0])
}
