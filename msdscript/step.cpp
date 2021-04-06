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
