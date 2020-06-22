#pragma once

#include <map>
#include <string>
#include <vector>

namespace AST
{
    class FunctionDeclarationNode;
}

class CompilationContext
{
public:
    ~CompilationContext();

    void ReportError(const std::string& errorText);

    AST::FunctionDeclarationNode* FindFunction(const std::string& functionName) const;
    void RegisterFunction(const std::string& functionName, AST::FunctionDeclarationNode* functionRootNode);

private:
    std::map<std::string, AST::FunctionDeclarationNode*> m_Functions;

    //TODO: Know the current line of code ?
};