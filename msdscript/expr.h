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

typedef enum {
  print_group_none,
  print_group_add,
  print_group_add_or_mult
} print_mode_t;

class Val;


class Expr{
public:
    virtual bool equals(Expr *other) = 0;
    virtual Val* interp() = 0;
    virtual Expr* subst(std::string str, Expr *other) = 0;
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
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class AddExpr: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    
    AddExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    //std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class MultExpr: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    MultExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    //std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class VarExpr: public Expr{
public:
    std::string str;
    VarExpr(std::string str);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class LetExpr: public Expr{
public:
    std::string variable;
    Expr *rhs;
    Expr *body;
    //int newLine;
    
    LetExpr(std::string variable, Expr *rhs, Expr *body);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
    //std::string to_string();
};

class BoolExpr: public Expr{
public:
    bool rep;
    
    BoolExpr(bool rep);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class EqExpr: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    
    EqExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};


class IfExpr: public Expr{
public:
    Expr *_if;
    Expr *_then;
    Expr *_else;
    
    IfExpr(Expr *_if, Expr *_then, Expr *_else);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class FunExpr: public Expr{
public:
    std::string formal_arg;
    Expr *body;
    
    FunExpr(std::string formal_arg, Expr *body);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};

class CallExpr: public Expr{
public:
    Expr *to_be_called;
    Expr *actual_arg;
    
    CallExpr(Expr *to_be_called, Expr *actual_arg);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS);
};
#endif /* expr_hpp */
