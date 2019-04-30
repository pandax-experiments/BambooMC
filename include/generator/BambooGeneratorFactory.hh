#ifndef BAMBOOGENERATORFACTORY_H
#define BAMBOOGENERATORFACTORY_H

#include "BambooGenerator.hh"
#include "simple_factory.hh"

using BambooGeneratorFactory =
    SimpleFactory<BambooGenerator, std::string, const G4String &>;

template <typename DerivedGenerator>
using GeneratorRegister =
    Register<BambooGenerator, std::string, DerivedGenerator, const G4String &>;

#endif // BAMBOOGENERATORFACTORY_H
