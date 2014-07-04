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
size_t IrDsymbol::count = 0;

void IrDsymbol::link()
{
    next = root;
    root = this;
    ++count;
}

void IrDsymbol::unlink()
{
    IrDsymbol** ppnode = &root;
    IrDsymbol* pnode = root;
    --count;
    while (pnode != this)
    {
        ppnode = &pnode->next;
        pnode = pnode->next;
    }
    *ppnode = next;
}

void IrDsymbol::resetAll()
{
    Logger::println("resetting %zu Dsymbols", count);
    IrDsymbol* it = root;

    while (it)
    {
        it->reset();
        it = it->next;
    }
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
