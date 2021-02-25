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
#include "val.h"

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
NumExpr::NumExpr(int rep) {
    this->rep = rep;
}

// Method to compare if this Num expression is equal to another expression
bool NumExpr::equals(Expr *other){
    NumExpr *other_num = dynamic_cast<NumExpr*>(other);
    if (other_num == NULL)
        return false;
    else
        return (this->rep == other_num->rep);
}

// Returns the interpreted int value of the Num
Val* NumExpr::interp(){
    return new NumVal(this->rep);
}

// Returns false because a Num is not a variable
// Nor does it contain a variable
bool NumExpr::has_variable(){
    return false;
}

// Returns this because it doesn't have a variable
// for comparison
Expr* NumExpr::subst(std::string subStr, Expr *other){
    return this;
};

// Prints number value to ostream
std::ostream& NumExpr::print(std::ostream& argument){
    return argument << this->rep;
}

// Prints number value to ostream (for pretty print)
std::ostream& NumExpr::pretty_print(std::ostream& argument){
    return argument << this->rep;
}

// Helper function to assist with pretty_print for Num Expressions
void NumExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    argument << this->rep;
}

// Add constructor
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Add expression is equal to another expression
bool AddExpr::equals(Expr *other){
    AddExpr *other_add = dynamic_cast<AddExpr*>(other);
    if (other_add == NULL)
        return false;
    else
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
}

// Returns the sum value as an int
Val* AddExpr::interp(){
    return lhs->interp()->add_to(rhs->interp());
}

// If any of the parameters for Add is a variable
// then it returns true
bool AddExpr::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

// Returns a new Add expr with substituted parameters if applicable
Expr* AddExpr::subst(std::string subStr, Expr *other){
    Expr *new_lhs = lhs->subst(subStr, other);
    Expr *new_rhs = rhs->subst(subStr, other);
    return new AddExpr(new_lhs,new_rhs);
};

// Print Addition expr to ostream
std::ostream& AddExpr::print(std::ostream& argument){
    
    argument << "(";
    this->lhs->print(argument);
    argument << "+";
    this->rhs->print(argument);
    argument << ")";
    return argument;
}

// Helper function to assist with pretty_print for Add Expressions
void AddExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
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
std::ostream& AddExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return std::cout;
}


// Mult constructor
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Mult expression is equal to another expression
bool MultExpr::equals(Expr *other){
    MultExpr *other_mult = dynamic_cast<MultExpr*>(other);
    if (other_mult == NULL)
        return false;
    else
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
}

// Returns the multiplication product value as an int
Val* MultExpr::interp(){
    return lhs->interp()->mult_by(rhs->interp());
}

// If any of the parameters for Mult is a variable
// then it returns true
bool MultExpr::has_variable(){
    return (lhs->has_variable() || rhs->has_variable());
}

// Returns a new Mult expr with substituted parameters if applicable
Expr* MultExpr::subst(std::string subStr, Expr *other){
    Expr *new_lhs = lhs->subst(subStr, other);
    Expr *new_rhs = rhs->subst(subStr, other);
    return new MultExpr(new_lhs,new_rhs);
};

// Print Multiplication expr to ostream
std::ostream& MultExpr::print(std::ostream& argument){
    argument << "(";
    this->lhs->print(argument);
    argument << "*";
    this->rhs->print(argument);
    argument << ")";
    return argument;
}

// Helper function to assist with pretty_print for Mult Expressions
void MultExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
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
std::ostream& MultExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}


// Var constructor
VarExpr::VarExpr(std::string str) {
    this->str = str;
}

// Method to compare if this Var expression is equal to another expression
bool VarExpr::equals(Expr *other){
    VarExpr*other_var = dynamic_cast<VarExpr*>(other);
    if (other_var == NULL)
        return false;
    else
        return (this->str == other_var->str);
}

// Cannot interpret an int value from a variable constant
// Therefore runtime error has to be thrown
Val* VarExpr::interp(){
    throw std::runtime_error("no value for variable");
}

// Checks to see if Var expression is a variable
bool VarExpr::has_variable(){
    return true;
}

// Substitute a specific variable with a given Expr of choice
Expr* VarExpr::subst(std::string subStr, Expr *other){
    if (this->str == subStr){
        return other;
    }
    else
        return this;
};

// Prints variable string to ostream
std::ostream& VarExpr::print(std::ostream& argument){
    return argument << str;
}

// Prints variable string to ostream (for pretty print)
std::ostream& VarExpr::pretty_print(std::ostream& argument){
    return argument << str;
}

// Helper function to assist with pretty_print for var Expressions
void VarExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    argument << str;
}

// _let constructor
LetExpr::LetExpr(std::string variable, Expr *rhs, Expr *body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
};

// Method to compare if this _let expression is equal to another expression
bool LetExpr::equals(Expr *other){
    LetExpr *other_let = dynamic_cast<LetExpr*>(other);
    if (other_let == NULL)
        return false;
    else
        return (this->rhs->equals(other_let->rhs) && this->body->equals(other_let->body) && (this->variable == other_let -> variable));
}

// Returns the calculated value as an int
Val* LetExpr::interp(){
    Val* rhs_val = rhs->interp();
    return body->subst(variable, rhs_val->to_expr())->interp();
}

// If the body parameter for _let has a variable
// then it returns true
bool LetExpr::has_variable(){
    return body->has_variable();
}

// Returns a new _let expr with substituted parameters if applicable
Expr* LetExpr::subst(std::string subStr, Expr *other){
//    if(subStr == variable){
//        Expr *new_rhs = rhs->subst(subStr,other);
//        return new LetExpr(variable,new_rhs, body);
//    }
//    else {
//        Expr *new_body = body->subst(subStr, other);
//        return new LetExpr(variable,rhs, new_body);
//    }

    Expr *new_rhs = rhs->subst(subStr,other);
    Expr *new_body = body;
    if(subStr != variable){
        new_body = body->subst(subStr, other);
    }
    return new LetExpr(variable,new_rhs, new_body);
      
};

// Print _let expr to ostream
std::ostream& LetExpr::print(std::ostream& argument){
    argument << "(_let " + variable + "=";
    this->rhs->print(argument);
    argument << " _in ";
    this->body->print(argument);
    argument << ")";
    return argument;
}

// Print function that uses parentheses only when needed
// For Multiplcation
std::ostream& LetExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}

// Helper function to assist with pretty_print for _let Expressions
void LetExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode != print_group_none && !alwaysRHS)
       argument << "(";
    int num1 = (int)argument.tellp();
    argument << "_let " + variable + " = ";
    this->rhs->pretty_print(argument);
    argument << "\n";
    int newLine = (int)argument.tellp();
    argument << std::string(num1-newLineLocation, ' ');
    argument << "_in  ";
    this->body->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    if (mode != print_group_none && !alwaysRHS)
         argument << ")";

}

// Different Exprs for testing purposes
NumExpr *null = NULL;
NumExpr *num1 = new NumExpr(1);
NumExpr *num2 = new NumExpr(2);
NumExpr *num3 = new NumExpr(3);
NumExpr *num4 = new NumExpr(4);
VarExpr *varx = new VarExpr("x");
VarExpr *vary = new VarExpr("y");


TEST_CASE ("equals") {
    CHECK( (new NumExpr(7))->equals(new NumExpr(7)) == true);
    CHECK( (new NumExpr(7))->equals(new NumExpr(6)) == false);
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true);
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false);
    CHECK( (new AddExpr(num1,num4))->equals(new AddExpr(num2,num3)) == false);
    CHECK( (new AddExpr(num1,num2))->equals(new AddExpr(num1,num2)) == true);
    CHECK( (new MultExpr(num1,num4))->equals(new MultExpr(num2,num3)) == false);
    CHECK( (new MultExpr(num1,num2))->equals(new MultExpr(num1,num2)) == true);
    CHECK( (new AddExpr(num1,varx))->equals(new AddExpr(num2,varx)) == false);
    CHECK( (new AddExpr(num1,vary))->equals(new AddExpr(num1,vary)) == true);
    CHECK( (new NumExpr(7))->equals(null) == false);
    CHECK( (new AddExpr(num1,num4))->equals(null) == false);
    CHECK( (new VarExpr("x"))->equals(null) == false);
    CHECK( (new MultExpr(num1,num4))->equals(null) == false);
    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->equals((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))) == true);
    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->equals((new MultExpr(new NumExpr(5), new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))) == false);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(null) == false);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == true);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == false);
    
}

TEST_CASE ("interp") {
    CHECK( (new NumExpr(7))->interp()->equals(new NumVal(7)));
    CHECK( (new AddExpr(num1,num2))->interp()->equals(new NumVal(3)));
    CHECK( (new MultExpr(num1,num2))->interp()->equals(new NumVal(2)));
    CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable");
    
    CHECK((new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->interp()->equals(new NumVal(73)));
    
    CHECK((new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->interp()->equals(new NumVal(20)));
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->interp()->equals(new NumVal(10)));
     
     
     CHECK((new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->interp()->equals(new NumVal(26)));
  
     
     CHECK((new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->interp() ->equals(new NumVal(6)));

     
     CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->interp()->equals(new NumVal(30)));
 
     
     CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->interp()->equals(new NumVal(6)));
    
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(11), new NumExpr(1))))->interp()->equals(new NumVal(12)));
    
    CHECK_THROWS_WITH((new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("y")))), new NumExpr(1)))->interp(), "no value for variable");
    
    CHECK_THROWS_WITH((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("z"), new NumExpr(1))))->interp(), "no value for variable");
  
}

TEST_CASE ("has_variable") {
    CHECK( (new NumExpr(7))->has_variable() == false);
    CHECK( (new VarExpr("x"))->has_variable() == true);
    CHECK( (new AddExpr(varx,num4))->has_variable() == true);
    CHECK( (new AddExpr(num1,num4))->has_variable() == false);
    CHECK( (new MultExpr(num2,vary))->has_variable() == true);
    CHECK( (new MultExpr(num1,num4))->has_variable() == false);
    CHECK((new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->has_variable() == true);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->has_variable() == true);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new NumExpr(1), new NumExpr(2) )), new NumExpr(2))))->has_variable() == false);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->has_variable() == true);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(2), new NumExpr(1))))->has_variable() == false);
    
}

TEST_CASE ("subst") {
    CHECK( (new VarExpr("x"))->subst("x", new VarExpr("b"))->equals(new VarExpr("b")) == true);
    CHECK( (new NumExpr(7))->subst("x", new NumExpr(8))->equals(new NumExpr(7)) == true);
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->interp()->equals(new NumVal(15)));
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))));
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->equals(new AddExpr(new NumExpr(8), new NumExpr(7))) == true);
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->equals(new MultExpr(new NumExpr(8), new NumExpr(7))) == true);
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) == true);
    CHECK( (new VarExpr("x"))->subst("y", new VarExpr("b"))->equals(new VarExpr("x")) == true);
    CHECK((new LetExpr("x", new AddExpr(new NumExpr(4), new VarExpr("x")), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("x", new AddExpr(new NumExpr(4), new VarExpr("y")),new VarExpr("x"))));
    CHECK((new LetExpr("y", new NumExpr(4), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("y",  new NumExpr(4) ,new VarExpr("y"))));
    CHECK((new LetExpr("x", new NumExpr(4), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("x",  new NumExpr(4) ,new VarExpr("x"))));
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(8)) ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(8)))) );
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new NumExpr(2)))) ->subst("y", new NumExpr(8)) ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(2)))) );
    CHECK( (new LetExpr("y", new VarExpr("x"), new VarExpr("x")))->subst("x", new NumExpr(3))->equals(new LetExpr("y", new NumExpr(3), new NumExpr(3))) );
}

TEST_CASE ("print") {
    std::ostringstream ss;
    
    (new AddExpr(num1,new AddExpr(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1+(2+3))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new AddExpr(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)+3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new AddExpr(num4,new AddExpr(num1,num3)))->to_string() == "(4+(1+3))");
    CHECK( (new AddExpr((new AddExpr(num2,num2)),new AddExpr(num1,num3)))->to_string() == "((2+2)+(1+3))");
    
    (new MultExpr(num1,new MultExpr(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1*(2*3))");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)*3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new MultExpr(num4,new AddExpr(num1,num3)))->to_string() == "(4*(1+3))");
    CHECK( (new MultExpr((new MultExpr(num2,num4)),new AddExpr(num1,num3)))->to_string() == "((2*4)*(1+3))");
    
    (new MultExpr(new AddExpr(num1,num4),new MultExpr(num3,num4)))->print(ss);
    CHECK(ss.str() == "((1+4)*(3*4))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new AddExpr(num1,varx),num3))->print(ss);
    CHECK(ss.str() == "((1+x)+3)");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,varx),new MultExpr(vary,num4)))->print(ss);
    CHECK(ss.str() == "((1+x)*(y*4))");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
     ss.str("");
     ss.clear();

     (new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->print(ss);
     CHECK(ss.str() == "((5*(_let x=5 _in x))+1)");
     ss.str("");
     ss.clear();

     (new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->print(ss);
     CHECK(ss.str() == "((_let x=5 _in x)+1)");
     ss.str("");
     ss.clear();

    (new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->print(ss);
     CHECK(ss.str() == "(5*(_let x=5 _in (x+1)))");
     ss.str("");
     ss.clear();

     (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in (x+1))");
     ss.str("");
     ss.clear();
    
    (new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7*(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7+(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
}

TEST_CASE ("pretty_print") {
    std::ostringstream ss;
    
    
    CHECK((new VarExpr("x"))->to_pretty_string() == "x");
    CHECK((new NumExpr(5))->to_pretty_string() == "5");
    CHECK((new AddExpr(num1,new AddExpr(num2,num3)))->to_pretty_string() == "1 + 2 + 3");
    
    (new AddExpr(new AddExpr(num1,num2),num3))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 2) + 3");
    ss.str("");
    ss.clear();
    
    (new AddExpr(num1,new MultExpr(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * 3");
    ss.str("");
    ss.clear();
    
    (new MultExpr(num1,new AddExpr(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 * (2 + 3)");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new MultExpr(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "(2 * 3) * 4");
    ss.str("");
    ss.clear();
    
    (new MultExpr(num2,new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,num4),new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 4) * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new AddExpr(num1,new MultExpr(num2,vary)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * y");
    ss.str("");
    ss.clear();
    
    
    (new MultExpr(new AddExpr(varx,num4),new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(x + 4) * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new MultExpr(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 + 4");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
    ss.str("");
    ss.clear();
    
    (new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->pretty_print(ss);
    CHECK(ss.str() == "5 * (_let x = 5\n     _in  x) + 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->pretty_print(ss);
    CHECK(ss.str() == "(_let x = 5\n _in  x) + 1");
    ss.str("");
    ss.clear();
    
   (new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->pretty_print(ss);
    CHECK(ss.str() == "5 * _let x = 5\n    _in  x + 1");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  x + 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))->pretty_print(ss);
    CHECK(ss.str() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");

    ss.str("");
    ss.clear();

    
    (new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->pretty_print(ss);
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
  Expr *let1 = (new LetExpr("x",
                        new NumExpr(1),
                        new AddExpr(new VarExpr("x"), new NumExpr(2))));
  CHECK( let1->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
        ->equals(let1) );

  // _let x = x
  // _in  x + 2  -> subst("x", y+3)
  // =
  // _let x = y+3
  // _in  x + 2
  Expr *let2 = (new LetExpr("x",
                        new VarExpr("x"),
                        new AddExpr(new VarExpr("x"), new NumExpr(2))));
  CHECK( let2->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
        ->equals(new LetExpr("x",
                         new AddExpr(new VarExpr("y"), new NumExpr(3)),
                         new AddExpr(new VarExpr("x"), new NumExpr(2)))) );
  
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

