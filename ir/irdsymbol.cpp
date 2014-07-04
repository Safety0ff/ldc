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

IrDsymbolMetadata::IrDsymbolMetadata()
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

bool IrDsymbolMetadata::isSet() const
{
    return (irAggr || irFunc || irGlobal || irLocal || irField);
}

IrVar* IrDsymbolMetadata::getIrVar()
{
    assert(irGlobal || irLocal || irField);
    return irGlobal ? static_cast<IrVar*>(irGlobal) : irLocal ? static_cast<IrVar*>(irLocal) : static_cast<IrVar*>(irField);
}

llvm::Value*& IrDsymbolMetadata::getIrValue() { return getIrVar()->value; }

std::unordered_map<IrDsymbol*, IrDsymbolMetadata> IrDsymbol::metadata;

void IrDsymbol::resetAll()
{
    Logger::println("resetting %zu Dsymbols", metadata.size());
    metadata.clear();
}

const IrDsymbolMetadata IrDsymbol::operator()() const
{
    auto it = metadata.find(const_cast<IrDsymbol*>(this));
    if (it != metadata.end())
        return it->second;
    return IrDsymbolMetadata();
}

IrDsymbolMetadata& IrDsymbol::get()
{
    return metadata[this];
}

llvm::Value*& IrDsymbol::getIrValue() { return metadata[this].getIrValue(); }

void IrDsymbol::set(IrDsymbolMetadata data)
{
    metadata[this] = data;
}

void IrDsymbol::setResolved()
{
    metadata[this].resolved = true;
}

void IrDsymbol::setDeclared()
{
    metadata[this].declared = true;
}

void IrDsymbol::setInitialized()
{
    metadata[this].initialized = true;
}

void IrDsymbol::setDefined()
{
    metadata[this].defined = true;
}

void IrDsymbol::setDModule(Module* mod)
{
    metadata[this].DModule = mod;
}

void IrDsymbol::setIrModule(IrModule* irmod)
{
    metadata[this].irModule = irmod;
}

void IrDsymbol::setIrAggr(IrAggr* irAggr)
{
    metadata[this].irAggr = irAggr;
}

void IrDsymbol::setIrFunc(IrFunction* irFunc)
{
    metadata[this].irFunc = irFunc;
}

void IrDsymbol::setIrGlobal(IrGlobal* irGlobal)
{
    metadata[this].irGlobal = irGlobal;
}

void IrDsymbol::setIrLocal(IrLocal* irLocal)
{
    metadata[this].irLocal = irLocal;
}

void IrDsymbol::setIrParam(IrParameter* irParam)
{
    metadata[this].irParam = irParam;
}

void IrDsymbol::setIrField(IrField* irField)
{
    metadata[this].irField = irField;
}