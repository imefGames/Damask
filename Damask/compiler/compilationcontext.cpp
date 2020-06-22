#include <compiler/compilationcontext.h>

#include <sstream>

CompilationContext::~CompilationContext()
{
    for (auto it : m_Functions)
    {
        delete it.second;
    }
    m_Functions.clear();
}

void CompilationContext::ReportError(const std::string& errorText)
{
    //TODO
}

AST::FunctionDeclarationNode* CompilationContext::FindFunction(const std::string& functionName) const
{
    auto foundIt{ m_Functions.find(functionName) };
    return (foundIt != m_Functions.end() ? foundIt->second : nullptr);
}

void CompilationContext::RegisterFunction(const std::string& functionName, AST::FunctionDeclarationNode* functionRootNode)
{
    if (!FindFunction(functionName))
    {
        m_Functions[functionName] = functionRootNode;
    }
    else
    {
        std::stringstream ss;
        ss << "Function " << functionName << " already exists.";
        ReportError(ss.str());
    }
}