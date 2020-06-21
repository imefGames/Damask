# Damask
 A quick and dirty attempt for an interpreted language.

The core of this project was mostly made over the course of a week-end.

Here's a list of issues I wish to address now that Damask can run some rudimentary instructions:
* There are many features programming languages have that need to be implemented here.
* Functions declarations are not taken into account.
* Operator precedence is not taken into account.
* The ***ASTExecutor*** visitor only handles S32 integers.
* The Lexer and the AST Builder need to report errors.
* There are many tokens the Lexer is not able to recongnise yet. 
* The code to build the AST is dirty. I will have to go over it to clean it.
* Building expressions in the AST is far from optimal. To do so I convert the parsed tokens from infix notation to RPN and only then do I build the tree.
* I should add some pooling system to reduce the impact of the frequent heap allocations when building the AST.
* The AST Node being an abstract class and using a Visitor pattern on the AST cause many virtual calls. I'll have to see if I can avoid those.
* Currently the code is executed via a Visitor. Later on I wish to run it via a virtual machine or even compile the code.
