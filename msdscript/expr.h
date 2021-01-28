//
//  expr.hpp
//  msdscript
//
//  Created by Harold Truong on 1/25/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>


class Expr{
public:
    virtual bool equals(Expr *other) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string str, Expr *other) = 0;
};

class Num: public Expr{
public:
    int val;
    Num(int val);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
};

class Add: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    
    Add(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
};

class Mult: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
};

class Var: public Expr{
public:
    std::string str;
    Var(std::string str);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
};

#endif /* expr_hpp */
