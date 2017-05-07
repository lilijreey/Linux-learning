/**
 * @file     parser.h
 *           
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     04/11/2017 01:04:11 PM
 *
 */

#pragma once

#include "ast.hpp"
#include <utility>

class ExprAST;

void HandleDefinition();
void HandleExtern();
void HandleTopLevelExpression();

int GetTokPrecedence();
int getNextToken();
void initBinOpPrec();

void MainLoop();

std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                       std::unique_ptr<ExprAST> LHS);
std::unique_ptr<FunctionAST> ParseDefinition() ;
std::unique_ptr<ExprAST> ParseExpression();
std::unique_ptr<PrototypeAST> ParseExtern() ;
std::unique_ptr<ExprAST> ParseIdentifierExpr();
std::unique_ptr<ExprAST> ParseNumberExpr();
std::unique_ptr<ExprAST> ParseParenExpr();
std::unique_ptr<ExprAST> ParsePrimary();
std::unique_ptr<PrototypeAST> ParsePrototype();
std::unique_ptr<FunctionAST> ParseTopLevelExpr();

