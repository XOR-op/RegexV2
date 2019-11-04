//
// Created by vortox on 12/10/19.
//

#ifndef REGEX_RPN_H
#define REGEX_RPN_H

#include <string>
namespace regu {
/*
 *  RPN: Reverse Polish Notation
 *       Support ()|*\
 *       User must guarantee that the expression is valid
 */

    std::string ProcessConcat(std::string s);

    std::string RPN(const std::string& s);
}
#endif //REGEX_RPN_H
