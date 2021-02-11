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

// Converts Expr to regular print string
std::string Expr::to_string(){
    std::ostringstream ss;
    this->print(ss);
    return ss.str();
}

// Converts Expr to pretty_pretty string
std::string Expr::to_pretty_string(){
    std::ostringstream ss;
    this->pretty_print(ss);
    return ss.str();
}

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
};

// Prints number value to ostream
std::ostream& Num::print(std::ostream& argument){
    return argument << this->val;
}

// Prints number value to ostream (for pretty print)
std::ostream& Num::pretty_print(std::ostream& argument){
    return argument << this->val;
}

// Helper function to assist with pretty_print for Num Expressions
void Num::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    argument << this->val;
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

// Helper function to assist with pretty_print for Add Expressions
void Add::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode >= print_group_add)
        argument << "(";
    this->lhs->pretty_print_at(print_group_add, argument, newLineLocation, false);
    argument << " + ";
    this->rhs->pretty_print_at(print_group_none, argument, newLineLocation, alwaysRHS);
    if (mode >= print_group_add)
        argument << ")";
}

// Print function that uses parentheses only when needed
// For Addition
std::ostream& Add::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return std::cout;
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

// Helper function to assist with pretty_print for Mult Expressions
void Mult::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode == print_group_add_or_mult)
        argument << "(";
    this->lhs->pretty_print_at(print_group_add_or_mult, argument, newLineLocation, false);
    argument << " * ";
    this->rhs->pretty_print_at(print_group_add, argument, newLineLocation, alwaysRHS);
    if (mode == print_group_add_or_mult)
        argument << ")";
}
// Print function that uses parentheses only when needed
// For Multiplcation
std::ostream& Mult::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
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
};

// Prints variable string to ostream
std::ostream& Var::print(std::ostream& argument){
    return argument << str;
}

// Prints variable string to ostream (for pretty print)
std::ostream& Var::pretty_print(std::ostream& argument){
    return argument << str;
}

// Helper function to assist with pretty_print for var Expressions
void Var::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    argument << str;
}

// _let constructor
_let::_let(std::string variable, Expr *rhs, Expr *body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
};

// Method to compare if this _let expression is equal to another expression
bool _let::equals(Expr *other){
    _let *other_let = dynamic_cast<_let*>(other);
    if (other_let == NULL)
        return false;
    else
        return (this->rhs->equals(other_let->rhs) && this->body->equals(other_let->body) && (this->variable == other_let -> variable));
}

// Returns the calculated value as an int
int _let::interp(){
    Expr* newBody = this->body->subst(variable, rhs);
    return newBody->interp();
}

// If the body parameter for _let has a variable
// then it returns true
bool _let::has_variable(){
    return body->has_variable();
}

// Returns a new _let expr with substituted parameters if applicable
Expr* _let::subst(std::string subStr, Expr *other){
    if(subStr == variable){
        Expr *new_rhs = rhs->subst(subStr,other);
        return new _let(variable,new_rhs, body);
    }
    else {
        Expr *new_body = body->subst(subStr, other);
        return new _let(variable,rhs, new_body);
    }
};

// Print _let expr to ostream
std::ostream& _let::print(std::ostream& argument){
    argument << "(_let " + variable + "=";
    this->rhs->print(argument);
    argument << " _in ";
    this->body->print(argument);
    argument << ")";
    return argument;
}

// Print function that uses parentheses only when needed
// For Multiplcation
std::ostream& _let::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}

// Helper function to assist with pretty_print for _let Expressions
void _let::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode != print_group_none && !alwaysRHS)
       argument << "(";
    int num1 = (int)argument.tellp();
    argument << "_let " + variable + " = ";
    this->rhs->print(argument);
    argument << "\n";
    int newLine = (int)argument.tellp();
    argument << std::string(num1-newLineLocation, ' ');
    argument << "_in  ";
    this->body->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    if (mode != print_group_none && !alwaysRHS)
         argument << ")";

}

// Different Exprs for testing purposes
Num *null = NULL;
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
    CHECK( (new Num(7))->equals(null) == false);
    CHECK( (new Add(num1,num4))->equals(null) == false);
    CHECK( (new Var("x"))->equals(null) == false);
    CHECK( (new Mult(num1,num4))->equals(null) == false);
    CHECK((new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->equals((new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))) == true);
    CHECK((new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->equals((new Mult(new Num(5), new _let("y", new Num(5), new Add(new Var("x"), new Num(1)))))) == false);
    CHECK((new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->equals(null) == false);
    CHECK((new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->equals(new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))) == true);
    CHECK((new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->equals(new _let("y", new Num(5), new Add(new Var("x"), new Num(1)))) == false);
    
}

TEST_CASE ("interp") {
    CHECK( (new Num(7))->interp() == 7);
    CHECK( (new Add(num1,num2))->interp() == 3);
    CHECK( (new Mult(num1,num2))->interp() == 2);
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable");
    
    CHECK((new Add(new Num(3),(new Mult(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->interp() == 73);
    
    CHECK((new Add(new Num(3),(new Add(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->interp() == 20);
    CHECK((new _let("x", new Num(5), new Add( new _let("y", new Num(3), new Add( new Var("y"), new Num(2) )), new Var("x"))))->interp() == 10);
     
     
     CHECK((new Add((new Mult(new Num(5), new _let("x", new Num(5), new Var("x")))), new Num(1)))->interp() == 26);
  
     
     CHECK((new Add(new _let("x", new Num(5), new Var("x")), new Num(1)))->interp() == 6);

     
     CHECK((new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->interp() == 30);
 
     
     CHECK((new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->interp() == 6);
    
    CHECK((new _let("x", new Num(5), new Add(new Num(11), new Num(1))))->interp() == 12);
    
    CHECK_THROWS_WITH((new Add((new Mult(new Num(5), new _let("x", new Num(5), new Var("y")))), new Num(1)))->interp(), "no value for variable");
    
    CHECK_THROWS_WITH((new _let("x", new Num(5), new Add(new Var("z"), new Num(1))))->interp(), "no value for variable");
  
}

TEST_CASE ("has_variable") {
    CHECK( (new Num(7))->has_variable() == false);
    CHECK( (new Var("x"))->has_variable() == true);
    CHECK( (new Add(varx,num4))->has_variable() == true);
    CHECK( (new Add(num1,num4))->has_variable() == false);
    CHECK( (new Mult(num2,vary))->has_variable() == true);
    CHECK( (new Mult(num1,num4))->has_variable() == false);
    CHECK((new Add(new Num(3),(new Add(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->has_variable() == true);
    CHECK((new _let("x", new Num(5), new Add( new _let("y", new Num(3), new Add( new Var("y"), new Num(2) )), new Var("x"))))->has_variable() == true);
    CHECK((new _let("x", new Num(5), new Add( new _let("y", new Num(3), new Add( new Num(1), new Num(2) )), new Num(2))))->has_variable() == false);
    CHECK((new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->has_variable() == true);
    CHECK((new _let("x", new Num(5), new Add(new Num(2), new Num(1))))->has_variable() == false);
    
}

TEST_CASE ("subst") {
    CHECK( (new Var("x"))->subst("x", new Var("b"))->equals(new Var("b")) == true);
    CHECK( (new Num(7))->subst("x", new Num(8))->equals(new Num(7)) == true);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->interp()==15);
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Add(new Var("y"), new Num(7))));
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Add(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Num(8))->equals(new Mult(new Num(8), new Num(7))) == true);
    CHECK( (new Mult(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Mult(new Var("y"), new Num(7))) == true);
    CHECK( (new Var("x"))->subst("y", new Var("b"))->equals(new Var("x")) == true);
    CHECK((new _let("x", new Add(new Num(4), new Var("x")), new Var("x")))->subst("x", new Var("y"))->equals(new _let("x", new Add(new Num(4), new Var("y")),new Var("x"))));
    CHECK((new _let("y", new Num(4), new Var("x")))->subst("x", new Var("y"))->equals(new _let("y",  new Num(4) ,new Var("y"))));
    CHECK((new _let("x", new Num(4), new Var("x")))->subst("x", new Var("y"))->equals(new _let("x",  new Num(4) ,new Var("x"))));
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
    
    (new _let("x", new Num(5), new Add( new _let("y", new Num(3), new Add( new Var("y"), new Num(2) )), new Var("x"))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
     ss.str("");
     ss.clear();

     (new Add((new Mult(new Num(5), new _let("x", new Num(5), new Var("x")))), new Num(1)))->print(ss);
     CHECK(ss.str() == "((5*(_let x=5 _in x))+1)");
     ss.str("");
     ss.clear();

     (new Add(new _let("x", new Num(5), new Var("x")), new Num(1)))->print(ss);
     CHECK(ss.str() == "((_let x=5 _in x)+1)");
     ss.str("");
     ss.clear();

    (new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->print(ss);
     CHECK(ss.str() == "(5*(_let x=5 _in (x+1)))");
     ss.str("");
     ss.clear();

     (new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in (x+1))");
     ss.str("");
     ss.clear();
    
    (new Add(new Num(3),(new Mult(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7*(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
    
    (new Add(new Num(3),(new Add(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7+(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
}

TEST_CASE ("pretty_print") {
    std::ostringstream ss;
    
    
    CHECK((new Var("x"))->to_pretty_string() == "x");
    CHECK((new Num(5))->to_pretty_string() == "5");
    CHECK((new Add(num1,new Add(num2,num3)))->to_pretty_string() == "1 + 2 + 3");
    
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
    
    (new Add(new Mult(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 + 4");
    ss.str("");
    ss.clear();
    
    (new _let("x", new Num(5), new Add( new _let("y", new Num(3), new Add( new Var("y"), new Num(2) )), new Var("x"))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
    ss.str("");
    ss.clear();
    
    (new Add((new Mult(new Num(5), new _let("x", new Num(5), new Var("x")))), new Num(1)))->pretty_print(ss);
    CHECK(ss.str() == "5 * (_let x = 5\n     _in  x) + 1");
    ss.str("");
    ss.clear();
    
    (new Add(new _let("x", new Num(5), new Var("x")), new Num(1)))->pretty_print(ss);
    CHECK(ss.str() == "(_let x = 5\n _in  x) + 1");
    ss.str("");
    ss.clear();
    
   (new Mult(new Num(5), new _let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->pretty_print(ss);
    CHECK(ss.str() == "5 * _let x = 5\n    _in  x + 1");
    ss.str("");
    ss.clear();
    
    (new _let("x", new Num(5), new Add(new Var("x"), new Num(1))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  x + 1");
    ss.str("");
    ss.clear();
    
    (new Add(new Num(3),(new Mult(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
    
    (new Add(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))->pretty_print(ss);
    CHECK(ss.str() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");

    ss.str("");
    ss.clear();

    
    (new Add(new Num(3),(new Add(new Num(7),(new _let("x", new Num(3), (new _let("x", new Num(5), new Add(new Var("x"), new _let("x", new Num(5), new Mult(new Var("x"), new Num(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
}

TEST_CASE( "more subst" ) {
  // _let x = 1
  // _in  x + 2  -> subst("x", y+3)
  // =
  // _let x = 1
  // _in  x + 2
  Expr *let1 = (new _let("x",
                        new Num(1),
                        new Add(new Var("x"), new Num(2))));
  CHECK( let1->subst("x", new Add(new Var("y"), new Num(3)))
        ->equals(let1) );

  // _let x = x
  // _in  x + 2  -> subst("x", y+3)
  // =
  // _let x = y+3
  // _in  x + 2
  Expr *let2 = (new _let("x",
                        new Var("x"),
                        new Add(new Var("x"), new Num(2))));
  CHECK( let2->subst("x", new Add(new Var("y"), new Num(3)))
        ->equals(new _let("x",
                         new Add(new Var("y"), new Num(3)),
                         new Add(new Var("x"), new Num(2)))) );
  
  // _let x = y
  // _in  x + 2 -> subst("y", 8)
  // =
  // _let x = 8
  // _in  x + 2
  
  // _let x = 8
  // _in  x + 2 + y -> subst("y", 9)
  // =
  // _let x = 8
  // _in  x + 2 + 9

  // y + y -> subst("y", 8)
  // =
  // 8 + 8

  // _let x = y
  // _in  x + y -> subst("y", 8)
  // =
  // _let x = 8
  // _in  x + 8

  // _let z = x
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = x
  // _in  z + 32
  
  // _let z = z
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = 0
  // _in  z + 32

  // _let z = z + 2
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = 0 + 2
  // _in  z + 32

}

