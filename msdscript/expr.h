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

class Expr{
public:
    virtual bool equals(Expr *other) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string str, Expr *other) = 0;
    virtual std::ostream& print(std::ostream& argument) = 0;
    std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument) = 0;
    std::ostream& pretty_print_at(print_mode_t mode, std::ostream& argument);
    
};

class Num: public Expr{
public:
    int val;
    Num(int val);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
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
    virtual std::ostream& print(std::ostream& argument);
    std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    std::ostream& pretty_print_at(print_mode_t mode, std::ostream& argument);
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
    virtual std::ostream& print(std::ostream& argument);
    std::string to_string();
    virtual std::ostream& pretty_print(std::ostream& argument);
    std::ostream& pretty_print_at(print_mode_t mode, std::ostream& argument);
};

class Var: public Expr{
public:
    std::string str;
    Var(std::string str);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr* subst(std::string str, Expr *other);
    virtual std::ostream& print(std::ostream& argument);
    virtual std::ostream& pretty_print(std::ostream& argument);
};

#endif /* expr_hpp */
