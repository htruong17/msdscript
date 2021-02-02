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
#include <sstream>

// Num constructor
Num::Num(int val) {
    this->val = val;
}

// Method to compare if this Num expression is equal to another expression
bool Num::equals(Expr *other){
    Num *other_num = dynamic_cast<Num*>(other);
    if (other_num == NULL)
        return false;
    else
        return (this->val == other_num->val);
}

// Returns the interpreted int value of the Num
int Num::interp(){
    return this->val;
}

// Returns false because a Num is not a variable
// Nor does it contain a variable
bool Num::has_variable(){
    return false;
}

// Returns this because it doesn't have a variable
// for comparison
Expr* Num::subst(std::string subStr, Expr *other){
    return this;
}

// Prints number value to ostream
std::ostream& Num::print(std::ostream& argument){
    return argument << val;
}

// Prints number value to ostream (for pretty print)
std::ostream& Num::pretty_print(std::ostream& argument){
    return argument << val;
}

// Add constructor
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Add expression is equal to another expression
bool Add::equals(Expr *other){
    Add *other_add = dynamic_cast<Add*>(other);
    if (other_add == NULL)
        return false;
    else
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
}

// Returns the sum value as an int
int Add::interp(){
    return this->lhs->interp() + this->rhs->interp();
}

// If any of the parameters for Add is a variable
// then it returns true
bool Add::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

// Returns a new Add expr with substituted parameters if applicable
Expr* Add::subst(std::string subStr, Expr *other){
    Expr *new_lhs = lhs->subst(subStr, other);
    Expr *new_rhs = rhs->subst(subStr, other);
    return new Add(new_lhs,new_rhs);
};

// Print Addition expr to ostream
std::ostream& Add::print(std::ostream& argument){
    
    argument << "(";
    this->lhs->print(argument);
    argument << "+";
    this->rhs->print(argument);
    argument << ")";
    return argument;
}

// Calls regular print and outputs a string
std::string Add::to_string(){
    std::ostringstream ss;
    this->print(ss);
    return ss.str();
}

// Print function that uses parentheses only when needed
// For Addition
std::ostream& Add::pretty_print(std::ostream& argument){
    print_mode_t mode;
    if (this->lhs->to_string().find(" ") != std::string::npos)
        mode = print_group_add_or_mult;
    else
        mode = print_group_none;
    this->lhs->pretty_print_at(mode, argument);
    argument << " + ";
    if (this->rhs->to_string().find("*") != std::string::npos)
        mode = print_group_none;
    else if (this->rhs->to_string().find(" ") != std::string::npos)
        mode = print_group_add_or_mult;
    else
        mode = print_group_none;
    this->rhs->pretty_print_at(mode, argument);
    return argument;
}

// Mult constructor
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Mult expression is equal to another expression
bool Mult::equals(Expr *other){
    Mult *other_mult = dynamic_cast<Mult*>(other);
    if (other_mult == NULL)
        return false;
    else
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
}

// Returns the multiplication product value as an int
int Mult::interp(){
    return this->lhs->interp() * this->rhs->interp();
}

// If any of the parameters for Mult is a variable
// then it returns true
bool Mult::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

// Returns a new Mult expr with substituted parameters if applicable
Expr* Mult::subst(std::string subStr, Expr *other){
    Expr *new_lhs = lhs->subst(subStr, other);
    Expr *new_rhs = rhs->subst(subStr, other);
    return new Mult(new_lhs,new_rhs);
};

// Print Multiplication expr to ostream
std::ostream& Mult::print(std::ostream& argument){
    argument << "(";
    this->lhs->print(argument);
    argument << "*";
    this->rhs->print(argument);
    argument << ")";
    return argument;
}

// Calls regular print and outputs a string
std::string Mult::to_string(){
    std::ostringstream ss;
    this->print(ss);
    return ss.str();
}

// Print function that uses parentheses only when needed
// For Multiplcation
std::ostream& Mult::pretty_print(std::ostream& argument){
    print_mode_t mode;
    if (this->lhs->to_string().find(" ") != std::string::npos)
        mode = print_group_add_or_mult;
    else
        mode = print_group_none;
    this->lhs->pretty_print_at(mode, argument);
    argument << " * ";
    if (this->rhs->to_string().find("+") != std::string::npos)
        mode = print_group_add;
    else
        mode = print_group_none;
    this->rhs->pretty_print_at(mode, argument);
    return argument;
}

// Helper function to assist with pretty_print.
// Breaksdown parentheses cases
std::ostream& Expr::pretty_print_at(print_mode_t mode, std::ostream& argument){
    if(mode == print_group_none){
        this->pretty_print(argument);
        return argument;
    }
    if(mode == print_group_add){
        argument << "(";
        this->pretty_print(argument);
        argument << ")";
        return argument;
    }
    if(mode == print_group_add_or_mult){
        argument << "(";
        this->pretty_print(argument);
        argument << ")";
        return argument;
    }
    return argument;
}

// Allows for Expr to call to_string in function
std::string Expr::to_string(){
    std::ostringstream ss;
    this->pretty_print(ss);
    return ss.str();
}

// Var constructor
Var::Var(std::string str) {
    this->str = str;
}

// Method to compare if this Var expression is equal to another expression
bool Var::equals(Expr *other){
    Var*other_var = dynamic_cast<Var*>(other);
    if (other_var == NULL)
        return false;
    else
        return (this->str == other_var->str);
}

// Cannot interpret an int value from a variable constant
// Therefore runtime error has to be thrown
int Var::interp(){
    throw std::runtime_error("no value for variable");
}

// Checks to see if Var expression is a variable
bool Var::has_variable(){
    return true;
}

// Substitute a specific variable with a given Expr of choice
Expr* Var::subst(std::string subStr, Expr *other){
    if (this->str == subStr){
        return other;
    }
    else
        return this;
}

// Prints variable string to ostream
std::ostream& Var::print(std::ostream& argument){
    return argument << str;
}

// Prints variable string to ostream (for pretty print)
std::ostream& Var::pretty_print(std::ostream& argument){
    return argument << str;
}


// Different Exprs for testing purposes
Num *num1 = new Num(1);
Num *num2 = new Num(2);
Num *num3 = new Num(3);
Num *num4 = new Num(4);
Var *varx = new Var("x");
Var *vary = new Var("y");

TEST_CASE ("equals") {
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
    
}

TEST_CASE ("interp") {
    CHECK( (new Num(7))->interp() == 7);
    CHECK( (new Add(num1,num2))->interp() == 3);
    CHECK( (new Mult(num1,num2))->interp() == 2);
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable");
}

TEST_CASE ("has_variable") {
    CHECK( (new Num(7))->has_variable() == false);
    CHECK( (new Var("x"))->has_variable() == true);
    CHECK( (new Add(varx,num4))->has_variable() == true);
    CHECK( (new Add(num1,num4))->has_variable() == false);
    CHECK( (new Mult(num2,vary))->has_variable() == true);
    CHECK( (new Mult(num1,num4))->has_variable() == false);
}

TEST_CASE ("subst") {
    CHECK( (new Var("x"))->subst("x", new Var("b"))->equals(new Var("b")) == true);
    CHECK( (new Num(7))->subst("x", new Num(8))->equals(new Num(7)) == true);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->interp()==15);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Add(new Var("y"), new Num(7))));
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Add(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Mult(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Mult(new Var("y"), new Num(7))) == true);
}

TEST_CASE ("print") {
    std::ostringstream ss;
    
    (new Add(num1,new Add(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1+(2+3))");
    ss.str("");
    ss.clear();
    
    (new Add(new Add(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)+3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new Add(num4,new Add(num1,num3)))->to_string() == "(4+(1+3))");
    CHECK( (new Add((new Add(num2,num2)),new Add(num1,num3)))->to_string() == "((2+2)+(1+3))");
    
    (new Mult(num1,new Mult(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1*(2*3))");
    ss.str("");
    ss.clear();
    
    (new Mult(new Add(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)*3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new Mult(num4,new Add(num1,num3)))->to_string() == "(4*(1+3))");
    CHECK( (new Mult((new Mult(num2,num4)),new Add(num1,num3)))->to_string() == "((2*4)*(1+3))");
    
    (new Mult(new Add(num1,num4),new Mult(num3,num4)))->print(ss);
    CHECK(ss.str() == "((1+4)*(3*4))");
    ss.str("");
    ss.clear();
    
    (new Add(new Add(num1,varx),num3))->print(ss);
    CHECK(ss.str() == "((1+x)+3)");
    ss.str("");
    ss.clear();
    
    (new Mult(new Add(num1,varx),new Mult(vary,num4)))->print(ss);
    CHECK(ss.str() == "((1+x)*(y*4))");
    ss.str("");
    ss.clear();
}

TEST_CASE ("pretty_print") {
    std::ostringstream ss;
    
    (new Add(num1,new Add(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 + (2 + 3)");
    ss.str("");
    ss.clear();
    
    (new Add(new Add(num1,num2),num3))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 2) + 3");
    ss.str("");
    ss.clear();
    
    (new Add(num1,new Mult(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * 3");
    ss.str("");
    ss.clear();
    
    (new Mult(num1,new Add(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 * (2 + 3)");
    ss.str("");
    ss.clear();
    
    (new Mult(new Mult(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "(2 * 3) * 4");
    ss.str("");
    ss.clear();
    
    (new Mult(num2,new Mult(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new Mult(new Add(num1,num4),new Mult(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 4) * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new Add(num1,new Mult(num2,vary)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * y");
    ss.str("");
    ss.clear();
    
    (new Mult(new Add(varx,num4),new Mult(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(x + 4) * 3 * 4");
    ss.str("");
    ss.clear();
}
