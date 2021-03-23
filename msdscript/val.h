//
//  val.hpp
//  msdscript
//
//  Created by Harold Truong on 2/24/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef val_h
#define val_h

#include "pointer.h"
#include <stdio.h>

class Expr;

class Val{
public:
//    std::string to_string();
//    std::string to_pretty_string();
//    virtual std::ostream& print(std::ostream& argument) = 0;
//    virtual std::ostream& pretty_print(std::ostream& argument) = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_by(PTR(Val) other_val) = 0;
    virtual bool equals(PTR(Val) other) = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;

};

class NumVal: public Val{
public:
    int rep;
    NumVal(int rep);
    virtual PTR(Expr) to_expr();
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) call(PTR(Val) actual_arg);
//    virtual std::ostream& print(std::ostream& argument);
//    virtual std::ostream& pretty_print(std::ostream& argument);
};


class BoolVal: public Val{
public:
    bool rep;
    BoolVal(bool rep);
    virtual PTR(Expr) to_expr();
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val{
public:
    std::string formal_arg;
    PTR(Expr) body;
    
    FunVal(std::string formal_arg, PTR(Expr) body);
    virtual PTR(Expr) to_expr();
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) call(PTR(Val) actual_arg);
};
#endif /* val_hpp */
