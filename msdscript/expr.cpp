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
#include "env.h"

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
bool NumExpr::equals(PTR(Expr) other){
    PTR(NumExpr) other_num = CAST(NumExpr)(other);
    if (other_num == NULL)
        return false;
    else
        return (this->rep == other_num->rep);
}

// Returns the interpreted int value of the Num
PTR(Val) NumExpr::interp(PTR(Env) env){
    return NEW(NumVal)(this->rep);
}


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
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Add expression is equal to another expression
bool AddExpr::equals(PTR(Expr) other){
    PTR(AddExpr) other_add = CAST(AddExpr)(other);
    if (other_add == NULL)
        return false;
    else
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
}

// Returns the sum value as an int
PTR(Val) AddExpr::interp(PTR(Env) env){
    return lhs->interp(env)->add_to(rhs->interp(env));
}


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
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Mult expression is equal to another expression
bool MultExpr::equals(PTR(Expr) other){
    PTR(MultExpr) other_mult = CAST(MultExpr)(other);
    if (other_mult == NULL)
        return false;
    else
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
}

// Returns the multiplication product value as an int
PTR(Val) MultExpr::interp(PTR(Env) env){
    return lhs->interp(env)->mult_by(rhs->interp(env));
}


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
bool VarExpr::equals(PTR(Expr) other){
    PTR(VarExpr) other_var = CAST(VarExpr)(other);
    if (other_var == NULL)
        return false;
    else
        return (this->str == other_var->str);
}

// Cannot interpret an int value from a variable constant
// Therefore runtime error has to be thrown
PTR(Val) VarExpr::interp(PTR(Env) env){
    return env->lookup(str);
}


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
LetExpr::LetExpr(std::string variable, PTR(Expr) rhs, PTR(Expr) body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
};

// Method to compare if this _let expression is equal to another expression
bool LetExpr::equals(PTR(Expr) other){
    PTR(LetExpr) other_let = CAST(LetExpr)(other);
    if (other_let == NULL)
        return false;
    else
        return (this->rhs->equals(other_let->rhs) && this->body->equals(other_let->body) && (this->variable == other_let -> variable));
}

// Returns the calculated value as an int
PTR(Val) LetExpr::interp(PTR(Env) env){
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(variable, rhs_val, env);
    return body->interp(new_env);
}


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
    //int newLine = (int)argument.tellp();
    this->rhs->pretty_print_at(print_group_none, argument, newLineLocation, alwaysRHS);
    argument << "\n";
    //newLine = (int)argument.tellp();
    int newLine = (int)argument.tellp();
    argument << std::string(num1-newLineLocation, ' ');
    //newLine = (int)argument.tellp();
    argument << "_in  ";
    this->body->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    if (mode != print_group_none && !alwaysRHS)
         argument << ")";

}

BoolExpr::BoolExpr(bool rep) {
    this->rep = rep;
}

// Method to compare if this Num expression is equal to another expression
bool BoolExpr::equals(PTR(Expr) other){
    PTR(BoolExpr) other_bool = CAST(BoolExpr)(other);
    if (other_bool == NULL)
        return false;
    else
        return (this->rep == other_bool->rep);
}

// Returns the interpreted int value of the Num
PTR(Val) BoolExpr::interp(PTR(Env) env){
    return NEW(BoolVal)(this->rep);
}


// Prints number value to ostream
std::ostream& BoolExpr::print(std::ostream& argument){
    if(this->rep == 0)
        return argument << "_false";
    else
        return argument << "_true";
}

// Prints number value to ostream (for pretty print)
std::ostream& BoolExpr::pretty_print(std::ostream& argument){
    if(this->rep == 0)
        return argument << "_false";
    else
        return argument << "_true";
}

// Helper function to assist with pretty_print for Num Expressions
void BoolExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if(this->rep == 0)
        argument << "_false";
    else
        argument << "_true";
}

// Add constructor
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// Method to compare if this Add expression is equal to another expression
bool EqExpr::equals(PTR(Expr) other){
    PTR(EqExpr) other_eq = CAST(EqExpr)(other);
    if (other_eq == NULL)
        return false;
    else
        return (this->lhs->equals(other_eq->lhs) && this->rhs->equals(other_eq->rhs));
}

// Returns the sum value as an int
PTR(Val) EqExpr::interp(PTR(Env) env){
    if (lhs->interp(env)->equals(rhs->interp(env)))
        return NEW(BoolVal)(true);
    else
        return NEW(BoolVal)(false);
}


// Print Addition expr to ostream
std::ostream& EqExpr::print(std::ostream& argument){
    
    argument << "(";
    this->lhs->print(argument);
    argument << "==";
    this->rhs->print(argument);
    argument << ")";
    return argument;
}

// Helper function to assist with pretty_print for Add Expressions
void EqExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode >= print_group_add)
        argument << "(";
    this->lhs->pretty_print_at(print_group_add, argument, newLineLocation, false);
    argument << " == ";
    this->rhs->pretty_print_at(print_group_none, argument, newLineLocation, alwaysRHS);
    if (mode >= print_group_add)
        argument << ")";
}

// Print function that uses parentheses only when needed
// For Addition
std::ostream& EqExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return std::cout;
}

IfExpr::IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else) {
    this->_if = _if;
    this->_then = _then;
    this->_else = _else;
};

// Method to compare if this _let expression is equal to another expression
bool IfExpr::equals(PTR(Expr) other){
    PTR(IfExpr) other_if = CAST(IfExpr)(other);
    if (other_if == NULL)
        return false;
    else
        return (this->_if->equals(other_if->_if) && this->_then->equals(other_if->_then) && (this->_else->equals(other_if->_else)));
}

// Returns the calculated value as an int
PTR(Val) IfExpr::interp(PTR(Env) env){
    if(_if->interp(env)->equals(NEW(BoolVal)(true)))
        return _then->interp(env);
    else if(_if->interp(env)->equals(NEW(BoolVal)(false)))
        return _else->interp(env);
    else
        throw std::runtime_error("If is a non-boolean value");
}


// Print _let expr to ostream
std::ostream& IfExpr::print(std::ostream& argument){
    argument << "(_if ";
    this->_if->print(argument);
    argument << " _then ";
    this->_then->print(argument);
    argument << " _else ";
    this->_else->print(argument);
    argument << ")";
    return argument;
}

// Print function that uses parentheses only when needed
// For Multiplcation
std::ostream& IfExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}

// Helper function to assist with pretty_print for _let Expressions
void IfExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode != print_group_none && !alwaysRHS)
       argument << "(";
    int num1 = (int)argument.tellp();
    argument << "_if ";
    this->_if->pretty_print(argument);
    argument << "\n";
    int newLine = (int)argument.tellp();
    argument << std::string(num1-newLineLocation, ' ');
    argument << "_then ";
    this->_then->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    argument << "\n";
    newLine = (int)argument.tellp();
    argument << std::string(num1-newLineLocation, ' ');
    argument << "_else ";
    this->_else->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    if (mode != print_group_none && !alwaysRHS)
         argument << ")";

}


FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) other){
    PTR(FunExpr) other_fun = CAST(FunExpr)(other);
    if (other_fun == NULL)
        return false;
    else
        return (this->formal_arg == (other_fun->formal_arg) && this->body->equals(other_fun->body));
}


PTR(Val) FunExpr::interp(PTR(Env) env){
    return NEW(FunVal)(formal_arg, body, env);
}


std::ostream& FunExpr::print(std::ostream& argument){
    
    argument << "(_fun (" + formal_arg + ") ";
//    argument << "(";
    this->body->print(argument);
    argument << ")";
    return argument;
}

void FunExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
    if (mode != print_group_none && !alwaysRHS)
       argument << "(";
    int num1 = (int)argument.tellp();
    argument << "_fun (" + formal_arg + ")";
    argument << "\n";
    int newLine = (int)argument.tellp();
    //int newLine  = 0;
    argument << std::string((num1-newLineLocation)+2, ' ');
    this->body->pretty_print_at(print_group_none, argument, newLine, alwaysRHS);
    if (mode != print_group_none && !alwaysRHS)
         argument << ")";

}

std::ostream& FunExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) other){
    PTR(CallExpr) other_call = CAST(CallExpr)(other);
    if (other_call == NULL)
        return false;
    else
        return (this->to_be_called->equals(other_call->to_be_called) && this->actual_arg->equals(other_call->actual_arg));
}


PTR(Val) CallExpr::interp(PTR(Env) env){
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}


std::ostream& CallExpr::print(std::ostream& argument){
    
   // argument << "(";
    this->to_be_called->print(argument);
    argument << "(";
    this->actual_arg->print(argument);
    argument << ")";
    return argument;
}

void CallExpr::pretty_print_at(print_mode_t mode, std::ostream& argument, int newLineLocation, bool alwaysRHS){
//    if (mode >= print_group_add)
//        argument << "(";
     //int newLine = (int)argument.tellp();
    this->to_be_called->pretty_print_at(print_group_add, argument, newLineLocation, false);
    argument << "(";
    
   
    this->actual_arg->pretty_print_at(print_group_none, argument, newLineLocation, alwaysRHS);
    argument << ")";
//    if (mode <= print_group_add)
//        argument << ")";
}

std::ostream& CallExpr::pretty_print(std::ostream& argument){
    this->pretty_print_at(print_group_none, argument, 0, true);
    return argument;
}
