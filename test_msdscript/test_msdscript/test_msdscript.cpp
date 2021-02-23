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
    }
    
    // Case for one argument
    else if (argc == 2){
        const char * const interp_argv[] = { argv[1], "--interp" };
        const char * const print_argv[] = { argv[1], "--print" };
        const char * const pretty_argv[] = { argv[1], "--pretty-print" };

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            // Mode results for executable
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pretty_result = exec_program(2, pretty_argv, in);

            // Looping print result output back as argument for interp
            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            // Looping pretty print result back into pretty print to see if is stays the same
            ExecResult pretty_again_result = exec_program(2, pretty_argv, pretty_result.out);

            // Test to see if print_result looping affected interp result
            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
            // Test to see if pretty print remains constant
            if (pretty_again_result.out != pretty_result.out)
                throw std::runtime_error("different result for pretty printed");
            // Error checking to ensure that argument input is valid
            if( print_result.exit_code == 1 || pretty_result.exit_code == 1){
                throw std::runtime_error("Program ended with exit code: 1");
            }
            // Shows if interp ran successfully for each cases
            if( interp_result.exit_code == 0){
                std::cout << "   interp okay\n";
            }
        }
        return 0;

    }
    
    // Case for two arguments for comparison
    else if (argc == 3){
        const char * const interp_argv_1[] = { argv[1], "--interp" };
        const char * const print_argv_1[] = { argv[1], "--print" };
        const char * const pretty_argv_1[] = { argv[1], "--pretty-print" };
        const char * const interp_argv_2[] = { argv[2], "--interp" };
        const char * const print_argv_2[] = { argv[2], "--print" };
        const char * const pretty_argv_2[] = { argv[2], "--pretty-print" };


        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";

            // Mode result for first executable
            ExecResult interp_result_1 = exec_program(2, interp_argv_1, in);
            ExecResult print_result_1 = exec_program(2, print_argv_1, in);
            ExecResult pretty_result_1 = exec_program(2, pretty_argv_1, in);

            // Mode result for second executable
            ExecResult interp_result_2 = exec_program(2, interp_argv_2, in);
            ExecResult print_result_2 = exec_program(2, print_argv_2, in);
            ExecResult pretty_result_2 = exec_program(2, pretty_argv_2, in);

            // USE FOR VISUAL PRINT OUT OF RESULTS
//            std::cout << "interp1: " << interp_result_1.out << "\n";
//            std::cout << "interp2: " << interp_result_2.out << "\n";
//            std::cout << "print1: " << print_result_1.out << "\n";
//            std::cout << "print2: " << print_result_2.out << "\n";
//            std::cout << "pretty1: " << pretty_result_1.out << "\n";
//            std::cout << "pretty2: " <<pretty_result_2.out << "\n";
            
            // Compare both interps
            if (interp_result_1.out != interp_result_2.out)
                throw std::runtime_error("different result for interp");
            // Compare both prints
            if (print_result_1.out != print_result_2.out)
                throw std::runtime_error("different result for print");
            // Compare both pretty prints
            if (pretty_result_1.out != pretty_result_2.out)
                throw std::runtime_error("different result for pretty print");
            // Error checking to see if both input arguments are valid
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
