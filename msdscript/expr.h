//
//  expr.hpp
//  msdscript
//
//  Created by Harold Truong on 1/25/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef expr_hpp
#define expr_hpp

#include "pointer.h"
#include <stdio.h>
#include <string>

typedef enum {
  print_group_none,
  print_group_add,
  print_group_add_or_mult
} print_mode_t;

class Val;


CLASS(Expr){
public:
    virtual bool equals(PTR(Expr) other) = 0;
    virtual PTR(Val) interp() = 0;
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other) = 0;
    virtual std::ostream& print(std::ostream& argument) = 0;
    std::string to_string();
    std::string to_pretty_string();
    virtual std::ostream& pretty_print(std::ostream& argument) = 0;
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS) = 0;
    
};

class NumExpr: public Expr{
public:
    int rep;
    NumExpr(int rep);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class AddExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    //std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class MultExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    //std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class VarExpr: public Expr{
public:
    std::string str;
    VarExpr(std::string str);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class LetExpr: public Expr{
public:
    std::string variable;
    PTR(Expr) rhs;
    PTR(Expr) body;
    //int newLine;
    
    LetExpr(std::string variable, PTR(Expr) rhs, PTR(Expr) body);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
    //std::string to_string();
};

class BoolExpr: public Expr{
public:
    bool rep;
    
    BoolExpr(bool rep);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class EqExpr: public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};


class IfExpr: public Expr{
public:
    PTR(Expr) _if;
    PTR(Expr) _then;
    PTR(Expr) _else;
    
    IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class FunExpr: public Expr{
public:
    std::string formal_arg;
    PTR(Expr) body;
    
    FunExpr(std::string formal_arg, PTR(Expr) body);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class CallExpr: public Expr{
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp();
    virtual PTR(Expr) subst(std::string str, PTR(Expr) other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};
#endif /* expr_hpp */
