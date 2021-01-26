//
//  expr.cpp
//  msdscript
//
//  Created by Harold Truong on 1/25/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "expr.h"
#include "catch.h"


    Num::Num(int val) {
        this->val = val;
    }

bool Num::equals(Expr *other){
    Num *other_num = dynamic_cast<Num*>(other);
    if (other_num == NULL)
        return false;
    else
        return (this->val == other_num->val);
}

    Add::Add(Expr *lhs, Expr *rhs) {
        this->lhs = lhs;
        this->rhs = rhs;
    }

bool Add::equals(Expr *other){
    Add *other_add = dynamic_cast<Add*>(other);
    if (other_add == NULL)
        return false;
    else
        return (this->lhs == other_add->lhs && this->rhs == other_add->rhs);
}

    
    Mult::Mult(Expr *lhs, Expr *rhs) {
        this->lhs = lhs;
        this->rhs = rhs;
    }

bool Mult::equals(Expr *other){
    Mult *other_mult = dynamic_cast<Mult*>(other);
    if (other_mult == NULL)
        return false;
    else
        return (this->lhs == other_mult->lhs && this->rhs == other_mult->rhs);
}

Var::Var(std::string str) {
    this->str = str;
    }

bool Var::equals(Expr *other){
    Var*other_var = dynamic_cast<Var*>(other);
    if (other_var == NULL)
        return false;
    else
        return (this->str == other_var->str);
}

TEST_CASE ("equals") {
    Num *num1 = new Num(1);
    Num *num2 = new Num(2);
    Num *num3 = new Num(3);
    Num *num4 = new Num(4);
    Var *varx = new Var("x");
    Var *vary = new Var("y");
    
    
    CHECK( (new Num(7))->equals(new Num(7)) == true);
    CHECK( (new Num(7))->equals(new Num(6)) == false);
    CHECK( (new Var("x"))->equals(new Var("x")) == true);
    CHECK( (new Var("x"))->equals(new Var("y")) == false);
    CHECK( (new Add(num1,num4))->equals(new Add(num2,num3)) == false);
    CHECK( (new Add(num1,num2))->equals(new Add(num1,num2)) == true);
    CHECK( (new Mult(num1,num4))->equals(new Mult(num2,num3)) == false);
    CHECK( (new Mult(num1,num2))->equals(new Mult(num1,num2)) == true);
    CHECK( (new Add(num1,varx))->equals(new Add(num2,varx)) == false);
    CHECK( (new Add(num1,vary))->equals(new Add(num1,vary)) == true) ;
}
