//
//  test_msdscript.cpp
//  test_msdscript
//
//  Created by Harold Truong on 2/22/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "exec.h"
#include <time.h>
#include "testgen.h"
#include <string>


int main(int argc, const char * argv[]) {
    srand((unsigned int)clock());
    
    if (argc ==1){
        std::cerr << "No argument given\n";
        
    } else if (argc == 2){
        const char * const interp_argv[] = { argv[1], "--interp" };
        const char * const print_argv[] = { argv[1], "--print" };
        const char * const pretty_argv[] = { argv[1], "--pretty-print" };
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pretty_result = exec_program(2, pretty_argv, in);

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            //ExecResult print_again_result = exec_program(2, print_argv, print_result.out);
            ExecResult pretty_again_result = exec_program(2, pretty_argv, pretty_result.out);
            
            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
            if (pretty_again_result.out != pretty_result.out)
                throw std::runtime_error("different result for pretty printed");
            if( print_result.exit_code == 1 || pretty_result.exit_code == 1){
                throw std::runtime_error("Program ended with exit code: 1");
            }
            if( interp_result.exit_code == 0){
                std::cout << "   interp okay\n";
            }
        }
        return 0;
        
    } else if (argc == 3){
        const char * const interp_argv_1[] = { argv[1], "--interp" };
        const char * const print_argv_1[] = { argv[1], "--print" };
        const char * const pretty_argv_1[] = { argv[1], "--pretty-print" };
        const char * const interp_argv_2[] = { argv[2], "--interp" };
        const char * const print_argv_2[] = { argv[2], "--print" };
        const char * const pretty_argv_2[] = { argv[2], "--pretty-print" };
        
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";
            
            ExecResult interp_result_1 = exec_program(2, interp_argv_1, in);
            ExecResult print_result_1 = exec_program(2, print_argv_1, in);
            ExecResult pretty_result_1 = exec_program(2, pretty_argv_1, in);
            
            ExecResult interp_result_2 = exec_program(2, interp_argv_2, in);
            ExecResult print_result_2 = exec_program(2, print_argv_2, in);
            ExecResult pretty_result_2 = exec_program(2, pretty_argv_2, in);

            if (interp_result_1.out != interp_result_2.out)
                throw std::runtime_error("different result for interp");
            if (print_result_1.out != print_result_2.out)
                throw std::runtime_error("different result for print");
            if (pretty_result_1.out != pretty_result_2.out)
                throw std::runtime_error("different result for pretty print");
            if( print_result_1.exit_code == 1 || print_result_2.exit_code == 1){
                throw std::runtime_error("Program ended with exit code: 1");
            }
        }
        return 0;
    }
    else
        std::cerr << "Invalid arguments given\n";
}


//static std::string random_bytes() {
//    std::string word = "";
//    for (int i = rand() % 32; i -- > 0;)
//        word += rand() % 256;
//    return word;
//}
