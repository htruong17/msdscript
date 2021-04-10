//
//  step.cpp
//  msdscript
//
//  Created by Harold Truong on 4/6/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "step.h"
#include "val.h"
#include "expr.h"
#include "cont.h"
#include "step.h"
#include "env.h"
#include "catch.h"
#include "parse.h"

Step::mode_t Step::mode;

PTR(Cont) Step::cont;
PTR(Expr) Step::expr;
PTR(Val) Step::val;
PTR(Env) Step::env;


PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = NEW(EmptyEnv)();
    Step::val = nullptr;
    Step::cont = Cont::done;
    while (1) {
        if (Step::mode == Step::interp_mode) {
            Step::expr->step_interp();
        } else {
            if (Step::cont == Cont::done) {
                return Step::val;
            } else {
                Step::cont->step_continue();
            }
            
        }
    }
    
}

TEST_CASE("Step"){
    CHECK( Step::interp_by_steps(parse_str("1"))->equals(NEW(NumVal)(1)) );
    CHECK( Step::interp_by_steps(parse_str("1+4"))->equals(NEW(NumVal)(5)) );
    CHECK( Step::interp_by_steps(parse_str("14*10"))->equals(NEW(NumVal)(140)) );
    CHECK( Step::interp_by_steps(parse_str("1==1"))->equals(NEW(BoolVal)("true")) );
    CHECK( Step::interp_by_steps(parse_str("_true"))->equals(NEW(BoolVal)("true")) );
    CHECK( Step::interp_by_steps(parse_str("_let f = _fun (x) x _in f(2)"))->equals(NEW(NumVal)(2)));
    CHECK( Step::interp_by_steps(parse_str(" _if (1==2) _then (1+5) _else 80+1)"))->equals(NEW(NumVal)(81)));
    CHECK( Step::interp_by_steps(parse_str(" _if (1==1) _then (1+5) _else 80+1)"))->equals(NEW(NumVal)(6)));
    CHECK( Step::interp_by_steps(parse_str("_let f = _fun (x) x _in  f(2)"))->equals(NEW(NumVal)(2)));
    CHECK_THROWS_WITH(Cont::done->step_continue(),"Cannot continue because it is done");
    
    
    

  
    
    
}
