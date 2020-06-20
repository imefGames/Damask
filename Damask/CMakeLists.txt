cmake_minimum_required (VERSION 3.8)

add_executable (Damask "Damask.cpp"
                       "lexer.h" "lexer.cpp"
                       "ast.h" "ast.cpp"
                       "executioncontext.h" "executioncontext.cpp"
                       "ast/node.h" "ast/node.cpp"
                       "ast/branchnode.h" "ast/branchnode.cpp"
                       "ast/functioncallnode.h" "ast/functioncallnode.cpp"
                       "ast/instructionsequencenode.h" "ast/instructionsequencenode.cpp"
                       "ast/operatornode.h" "ast/operatornode.cpp"
                       "ast/rawvaluenode.h" "ast/rawvaluenode.cpp"
                       "ast/variabledeclarationnode.h" "ast/variabledeclarationnode.cpp"
                       "ast/visitor/astvisitor.h"
                       "ast/visitor/astdisplayer.h" "ast/visitor/astdisplayer.cpp"
                       "ast/visitor/astdebugexecutor.h" "ast/visitor/astdebugexecutor.cpp"
                       )

set_property(TARGET Damask PROPERTY CXX_STANDARD 17)