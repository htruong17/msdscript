//
//  val.cpp
//  msdscript
//
//  Created by Harold Truong on 2/24/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "catch.h"
#include <stdexcept>
#include "val.h"
#include "expr.h"
#include "env.h"
#include <string>
#include "step.h"
//#include "parse.h"
//#include <sstream>



//std::string Val::to_string(){
//    std::ostringstream ss;
//    this->print(ss);
//    return ss.str();
//}
//
//// Converts Expr to pretty_pretty string
//std::string Val::to_pretty_string(){
//    std::ostringstream ss;
//    this->pretty_print(ss);
//    return ss.str();
//}
//
//std::ostream& NumVal::print(std::ostream& argument){
//    return argument << this->rep;
//}
//
//// Prints number value to ostream (for pretty print)
//std::ostream& NumVal::pretty_print(std::ostream& argument){
//    return argument << this->rep;
//}

NumVal::NumVal(int rep) {
    this->rep = rep;
}


PTR(Val) NumVal::add_to(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL)
        throw std::runtime_error("add of non-number");
    return NEW(NumVal)(rep + other_num->rep);
}

PTR(Val) NumVal::mult_by(PTR(Val) other_val){
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL)
        throw std::runtime_error("mult of non-number");
    return NEW(NumVal)(rep * other_num->rep);
}

bool NumVal::equals(PTR(Val) other){
    PTR(NumVal) other_numval = CAST(NumVal)(other);
    if (other_numval == NULL)
        return false;
    else
        return (this->rep == other_numval->rep);
}

std::string NumVal::to_string(){
    return std::to_string(rep);
}

PTR(Val) NumVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("call error for NumVal");
}

void NumVal::call_step(PTR(Val) actual_arg, PTR(Cont) rest) {
    throw std::runtime_error("cannot call on Num");
}

bool NumVal::is_true(){
    throw std::runtime_error("numbers cannot be true or false");
}

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}


PTR(Val) BoolVal::add_to(PTR(Val) other_val){
    throw std::runtime_error("cannot add with boolean");
}

PTR(Val) BoolVal::mult_by(PTR(Val) other_val){
    throw std::runtime_error("cannot multiply with boolean");
}

bool BoolVal::equals(PTR(Val) other){
    PTR(BoolVal) other_boolval = CAST(BoolVal)(other);
    if (other_boolval == NULL)
        return false;
    else
        return (this->rep == other_boolval->rep);
}

std::string BoolVal::to_string(){
    if (rep)
        return "_true";
    else
        return "_false";
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg){
    throw std::runtime_error("call error for BoolVal");
}

void BoolVal::call_step(PTR(Val) actual_arg, PTR(Cont) rest) {
    throw std::runtime_error("cannot call on Bool");
}

bool BoolVal::is_true(){
    return rep;
}

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env){
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}


PTR(Val) FunVal::add_to(PTR(Val) other_val){
    throw std::runtime_error("add error for FunVal");
}

PTR(Val) FunVal::mult_by(PTR(Val) other_val){
    throw std::runtime_error("mult error for FunVal");
}

bool FunVal::equals(PTR(Val) other){
    PTR(FunVal) other_funval = CAST(FunVal)(other);
    if (other_funval == NULL)
        return false;
    else
        return (this->formal_arg == other_funval->formal_arg && this->body->equals(other_funval->body));
}

std::string FunVal::to_string(){
    return "(_fun (" + formal_arg + ") " + body->to_string() + ")";
}

PTR(Val) FunVal::call(PTR(Val) actual_arg){
//    PTR(FunVal) other_fun = dynamic_cast<PTR(FunVal)>(actual_arg);
//    if (other_fun == NULL)
//        throw std::runtime_error("Call error for FunVal");
    return this->body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
    //return this->body
}

void FunVal::call_step(PTR(Val) actual_arg, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg, env);
    Step::cont = rest;
}

bool FunVal::is_true(){
    throw std::runtime_error("Functions cannot be true or false");
}


TEST_CASE ("NumVal equals") {
    CHECK((NEW(AddExpr)((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")))), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NULL) == false);
    CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->interp(Env::empty) ->equals(NULL) == false);
}

TEST_CASE ("NumVal add_to") {
    CHECK_THROWS_WITH( (NEW(MultExpr)(NEW(NumExpr)(5),NEW(NumExpr)(2)))->interp(Env::empty)->add_to(NULL),"add of non-number");
    CHECK_THROWS_WITH( (NEW(AddExpr)(NEW(NumExpr)(5),NEW(NumExpr)(2)))->interp(Env::empty)->mult_by(NULL),"mult of non-number");
    
    //CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NULL))->interp(Env::empty));
}


TEST_CASE ("Bool Val Equals") {
    CHECK((NEW(BoolVal)(true))->equals(NULL) == false);
}

TEST_CASE ("FunVal Tests") {
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->add_to((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))),"add error for FunVal");
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->mult_by((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))),"mult error for FunVal");
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->equals((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))));
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->equals(NULL) == false);
}

TEST_CASE("Call Tests"){
    CHECK_THROWS_WITH((NEW(NumVal)(3))->call(NEW(NumVal)(5)),"call error for NumVal");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->call(NEW(NumVal)(5)),"call error for BoolVal");
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->call(NEW(NumVal)(5))->equals(NEW(NumVal)(10)));
}

TEST_CASE("to_string"){
    CHECK((NEW(NumVal)(24))->to_string() == "24");
    CHECK((NEW(BoolVal)(true))->to_string() == "_true");
    CHECK((NEW(BoolVal)(false))->to_string() == "_false");
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)),Env::empty))->to_string() == "(_fun (x) (x+5))");
}
