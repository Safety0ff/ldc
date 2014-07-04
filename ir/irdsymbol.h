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

struct IrDsymbol
{
    static void resetAll();

    // overload all of these to make sure
    // the static list is up to date
    IrDsymbol();
    IrDsymbol(const IrDsymbol& s);
    ~IrDsymbol();

    void reset();

private:
    // Intrusive circular doubly linked list
    IrDsymbol* next;
    IrDsymbol* prev;
    static IrDsymbol* root;
    void link();
    void unlink();
public:

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
    IrVar* getIrVar();

    bool resolved;
    bool declared;
    bool initialized;
    bool defined;

    llvm::Value*& getIrValue();

    bool isSet();
};

#endif
