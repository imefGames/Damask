cmake_minimum_required (VERSION 3.8)

add_executable (Damask "Damask.cpp"
                       "ast/lexer.h" "ast/lexer.cpp"
                       "ast/astbuilder.h" "ast/astbuilder.cpp"
                       "ast/compilationcontext.h" "ast/compilationcontext.cpp"
                       "ast/node/node.h" "ast/node/node.cpp"
                       "ast/node/branchnode.h" "ast/node/branchnode.cpp"
                       "ast/node/functioncallnode.h" "ast/node/functioncallnode.cpp"
                       "ast/node/functiondeclarationnode.h" "ast/node/functiondeclarationnode.cpp"
                       "ast/node/instructionsequencenode.h" "ast/node/instructionsequencenode.cpp"
                       "ast/node/loopnode.h" "ast/node/loopnode.cpp"
                       "ast/node/operatornode.h" "ast/node/operatornode.cpp"
                       "ast/node/rawvaluenode.h" "ast/node/rawvaluenode.cpp"
                       "ast/node/variabledeclarationnode.h" "ast/node/variabledeclarationnode.cpp"
                       "ast/node/visitor/astvisitor.h"
                       "ast/node/visitor/astdisplayer.h" "ast/node/visitor/astdisplayer.cpp"
                       "ast/node/visitor/astdebugexecutor.h" "ast/node/visitor/astdebugexecutor.cpp"
                       "virtualmachine/instructions.h" "virtualmachine/instructions.cpp"
                       "virtualmachine/vmcontext.h" "virtualmachine/vmcontext.cpp"
                       )

set_property(TARGET Damask PROPERTY CXX_STANDARD 17)