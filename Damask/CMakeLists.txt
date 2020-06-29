cmake_minimum_required (VERSION 3.8)

add_executable (Damask "Damask.cpp"
                       "compiler/lexer.h" "compiler/lexer.cpp"
                       "compiler/astbuilder.h" "compiler/astbuilder.cpp"
                       "compiler/compilationcontext.h" "compiler/compilationcontext.cpp"
                       "compiler/ast/node.h" "compiler/ast/node.cpp"
                       "compiler/ast/branchnode.h" "compiler/ast/branchnode.cpp"
                       "compiler/ast/functioncallnode.h" "compiler/ast/functioncallnode.cpp"
                       "compiler/ast/functiondeclarationnode.h" "compiler/ast/functiondeclarationnode.cpp"
                       "compiler/ast/instructionsequencenode.h" "compiler/ast/instructionsequencenode.cpp"
                       "compiler/ast/loopnode.h" "compiler/ast/loopnode.cpp"
                       "compiler/ast/operatornode.h" "compiler/ast/operatornode.cpp"
                       "compiler/ast/rawvaluenode.h" "compiler/ast/rawvaluenode.cpp"
                       "compiler/ast/variabledeclarationnode.h" "compiler/ast/variabledeclarationnode.cpp"
                       "compiler/ast/visitor/astvisitor.h"
                       "compiler/ast/visitor/astdisplayer.h" "compiler/ast/visitor/astdisplayer.cpp"
                       "compiler/ast/visitor/astdebugexecutor.h" "compiler/ast/visitor/astdebugexecutor.cpp"
                       "virtualmachine/instructions.h" "virtualmachine/instructions.cpp"
                       "virtualmachine/vmcontext.h" "virtualmachine/vmcontext.cpp"
                       )

set_property(TARGET Damask PROPERTY CXX_STANDARD 17)