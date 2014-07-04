//===-- irdsymbol.cpp -----------------------------------------------------===//
//
//                         LDC – the LLVM D compiler
//
// This file is distributed under the BSD-style LDC license. See the LICENSE
// file for details.
//
//===----------------------------------------------------------------------===//

#include "gen/llvm.h"
#include "gen/logger.h"
#include "ir/irdsymbol.h"
#include "ir/irvar.h"

// redundant initializers
IrDsymbol* IrDsymbol::root = NULL;

void IrDsymbol::link()
{
    if (root)
    {
        next = root;
        prev = root->prev;
        root->prev->next = this;
        root->prev = this;
    }
    else
    {
        prev = this;
        next = this;
    }
    root = this;
}

void IrDsymbol::unlink()
{
    if (next != this)
    {
        prev->next = next;
        next->prev = prev;
        root = next;
    }
    else
        root = NULL;
}

void IrDsymbol::resetAll()
{
    if (!root)
        return;

    IrDsymbol* it = root;
    size_t count = 0;
    do
    {
        it->reset();
        it = it->next;
        ++count;
    } while (it != root);
    Logger::println("reset %zu Dsymbols", count);
}

IrDsymbol::IrDsymbol()
{
    link();
    reset();
}

IrDsymbol::IrDsymbol(const IrDsymbol& s)
{
    link();
    DModule = s.DModule;
    irModule = s.irModule;
    irAggr = s.irAggr;
    irFunc = s.irFunc;
    resolved = s.resolved;
    declared = s.declared;
    initialized = s.initialized;
    defined = s.defined;
    irGlobal = s.irGlobal;
    irLocal = s.irLocal;
    irField = s.irField;
}

IrDsymbol::~IrDsymbol()
{
    unlink();
}

void IrDsymbol::reset()
{
    DModule = NULL;
    irModule = NULL;
    irAggr = NULL;
    irFunc = NULL;
    irGlobal = NULL;
    irLocal = NULL;
    irField = NULL;
    resolved = declared = initialized = defined = false;
}

bool IrDsymbol::isSet()
{
    return (irAggr || irFunc || irGlobal || irLocal || irField);
}

IrVar* IrDsymbol::getIrVar()
{
    assert(irGlobal || irLocal || irField);
    return irGlobal ? static_cast<IrVar*>(irGlobal) : irLocal ? static_cast<IrVar*>(irLocal) : static_cast<IrVar*>(irField);
}

llvm::Value*& IrDsymbol::getIrValue() { return getIrVar()->value; }
