//
//  env.cpp
//  msdscript
//
//  Created by Harold Truong on 3/30/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "env.h"
#include "pointer.h"
#include "catch.h"
#include "val.h"

EmptyEnv::EmptyEnv(){}

PTR(Env) Env::empty = NEW(EmptyEnv)();

PTR(Val) EmptyEnv::lookup(std::string find_name){
    throw std::runtime_error("free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest){
    this->name = name;
    this->val = val;
    this->rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string find_name){
    if (find_name == name)
        return val;
    else
        return rest->lookup(find_name);
}

TEST_CASE("EmptyEnv"){
    CHECK_THROWS_WITH((NEW(EmptyEnv)())->lookup("x"), "free variable: x");
    CHECK_THROWS_WITH((Env::empty)->lookup("x"), "free variable: x");
}

TEST_CASE("EmtendedEnv"){
    CHECK((NEW(ExtendedEnv("name", NEW(NumVal(5)), Env::empty)))->lookup("name")->equals(NEW(NumVal(5))));
    CHECK_THROWS_WITH((NEW(ExtendedEnv("name", NEW(NumVal(5)), Env::empty)))->lookup("no name"), "free variable: no name");
}

