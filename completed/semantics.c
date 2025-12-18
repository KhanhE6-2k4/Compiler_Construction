/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

Object* lookupObject(char *name) {
  // TODO
  Scope* scope = symtab->currentScope;
  
  while (scope != NULL) {
    Object* obj = findObject(scope->objList, name);
    if (obj != NULL) {
      return obj;
    }
    scope = scope->outer;
  }
  Object* globalObj = findObject(symtab->globalObjectList, name);
  if (globalObj != NULL) {
    return globalObj;
  }
  return NULL;
}

void checkFreshIdent(char *name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = findObject(symtab->currentScope->objList, name);
  if (obj != NULL) {
    error(ERR_DUPLICATE_IDENT, ln, cn);
  }
}

Object* checkDeclaredIdent(char* name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT, ln, cn);
  }
  return obj;
}

Object* checkDeclaredConstant(char* name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_CONSTANT, ln, cn);
  }
  else if (obj->kind != OBJ_CONSTANT) {
    error(ERR_UNDECLARED_CONSTANT, ln, cn);
  }
  return obj;
}

Object* checkDeclaredType(char* name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_TYPE, ln, cn);
  }
  else if (obj->kind != OBJ_TYPE) {
    error(ERR_UNDECLARED_TYPE, ln, cn);
  }
  return obj;
}

Object* checkDeclaredVariable(char* name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_VARIABLE, ln, cn);
  }
  else if (obj->kind != OBJ_VARIABLE) {
    error(ERR_UNDECLARED_VARIABLE, ln, cn);
  }
  return obj;
}

Object* checkDeclaredFunction(char* name) {
  // TODO
  int ln = currentToken->lineNo;  
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_FUNCTION, ln, cn);
  }
  else if (obj->kind != OBJ_FUNCTION) {
    error(ERR_UNDECLARED_FUNCTION, ln, cn);
  }
  return obj;
}

Object* checkDeclaredProcedure(char* name) {
  // TODO
  int ln = currentToken->lineNo;  
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_PROCEDURE, ln, cn);
  }
  else if (obj->kind != OBJ_PROCEDURE) {
    error(ERR_UNDECLARED_PROCEDURE, ln, cn);
  }
  return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
  // TODO
  int ln = currentToken->lineNo;
  int cn = currentToken->colNo;

  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT, ln, cn);
  }
  else if (obj->kind != OBJ_VARIABLE && obj->kind != OBJ_PARAMETER && obj->kind != OBJ_FUNCTION) {
    error(ERR_INVALID_LVALUE, ln, cn);
  }
  return obj;
}

