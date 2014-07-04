//===-- ir/irdsymbol.h - Codegen state for D symbols ------------*- C++ -*-===//
//
//                         LDC – the LLVM D compiler
//
// This file is distributed under the BSD-style LDC license. See the LICENSE
// file for details.
//
//===----------------------------------------------------------------------===//
//
// Represents the status of a D symbol on its way though the codegen process.
//
//===----------------------------------------------------------------------===//

#ifndef LDC_IR_IRDSYMBOL_H
#define LDC_IR_IRDSYMBOL_H

struct IrModule;
struct IrFunction;
struct IrAggr;
struct IrGlobal;
struct IrLocal;
struct IrParameter;
struct IrField;
struct IrVar;
class Dsymbol;
class Module;

namespace llvm {
    class Value;
}

struct IrDsymbolMetadata
{
    IrDsymbolMetadata();
    Module* DModule;
    IrModule* irModule;
    IrAggr* irAggr;
    IrFunction* irFunc;
    IrGlobal* irGlobal;
    union {
        IrLocal* irLocal;
        IrParameter *irParam;
    };
    IrField* irField;

    bool resolved;
    bool declared;
    bool initialized;
    bool defined;

    IrVar* getIrVar();
    llvm::Value*& getIrValue();
    bool isSet() const;
};

#include <unordered_map>

struct IrDsymbol
{
    static std::unordered_map<IrDsymbol*, IrDsymbolMetadata> metadata;
    static void resetAll();

    const IrDsymbolMetadata operator()() const;
    IrDsymbolMetadata& get();
    llvm::Value*& getIrValue();

    void set(IrDsymbolMetadata);
    void setResolved();
    void setDeclared();
    void setInitialized();
    void setDefined();

    void setDModule(Module*);
    void setIrModule(IrModule*);
    void setIrAggr(IrAggr*);
    void setIrFunc(IrFunction*);
    void setIrGlobal(IrGlobal*);
    void setIrLocal(IrLocal*);
    void setIrParam(IrParameter*);
    void setIrField(IrField*);
};

#endif
