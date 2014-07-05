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
IrDsymbol* IrDsymbol::head = NULL;
size_t IrDsymbol::count = 0;

void IrDsymbol::dirty()
{
    if (next)
        return; // already linked
    next = head;
    head = this;
    ++count;
}

void IrDsymbol::resetAll()
{
    Logger::println("resetting %zu Dsymbols", count);
    IrDsymbol* it = head;

    while (it)
    {
        IrDsymbol* tmp = it->next;
        it->reset();
        it = tmp;
    }
    head = NULL;
    count = 0;
}

IrDsymbol::IrDsymbol()
{
    reset();
}

IrDsymbol::IrDsymbol(const IrDsymbol& s)
{
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
    next = NULL;
    if (s.next)
        dirty();
}

IrDsymbol::~IrDsymbol()
{
    assert(!next);
}

void IrDsymbol::reset()
{
    memset(this, 0, sizeof(*this));
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
