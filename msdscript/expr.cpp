//
//  expr.cpp
//  msdscript
//
//  Created by Harold Truong on 1/25/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "expr.h"
#include "catch.h"
#include <stdexcept>
#include <iostream>


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

int Num::interp(){
    return this->val;
}

bool Num::has_variable(){
    return false;
}

Expr* Num::subst(std::string subStr, Expr *other){
    return this;
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
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
}

int Add::interp(){
    return this->lhs->interp() + this->rhs->interp();
}

bool Add::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

Expr* Add::subst(std::string subStr, Expr *other){
    if (lhs->equals(new Var(subStr))){
        this->lhs = other;
    }
    if (rhs->equals(new Var(subStr))){
        this->rhs = other;
    }
    return this;
};

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *other){
    Mult *other_mult = dynamic_cast<Mult*>(other);
    if (other_mult == NULL)
        return false;
    else
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
}

int Mult::interp(){
    return this->lhs->interp() * this->rhs->interp();
}

bool Mult::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

Expr* Mult::subst(std::string subStr, Expr *other){
    if (lhs->equals(new Var(subStr))){
        this->lhs = other;
    }
    if (rhs->equals(new Var(subStr))){
        this->rhs = other;
    }
    return new Mult(lhs,rhs);
};

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

int Var::interp(){
    throw std::runtime_error("no value for variable");
}

bool Var::has_variable(){
    return true;
}

Expr* Var::subst(std::string subStr, Expr *other){
    if (this->str == subStr){
        return other;
    }
    else
        return this;
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
    CHECK( (new Add(num1,vary))->equals(new Add(num1,vary)) == true);
    CHECK( (new Num(7))->interp() == 7);
    CHECK( (new Add(num1,num2))->interp() == 3);
    CHECK( (new Mult(num1,num2))->interp() == 2);
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable");
    CHECK( (new Num(7))->has_variable() == false);
    CHECK( (new Var("x"))->has_variable() == true);
    CHECK( (new Add(varx,num4))->has_variable() == true);
    CHECK( (new Add(num1,num4))->has_variable() == false);
    CHECK( (new Mult(num2,vary))->has_variable() == true);
    CHECK( (new Mult(num1,num4))->has_variable() == false);
    CHECK( (new Var("x"))->subst("x", new Var("b"))->equals(new Var("b")) == true);
    CHECK( (new Num(7))->subst("x", new Num(8))->equals(new Num(7)) == true);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->interp()==15);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Add(new Var("y"), new Num(7))) == true);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Add(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Mult(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Mult(new Var("y"), new Num(7))) == true);
}
