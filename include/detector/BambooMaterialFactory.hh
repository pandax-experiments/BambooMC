#ifndef BAMBOOMATERIALFACTORY_H
#define BAMBOOMATERIALFACTORY_H

#include "BambooMaterial.hh"

#include "simple_factory.hh"

using BambooMaterialFactory =
    SimpleFactory<BambooMaterial, std::string, const G4String &>;

template <typename DerivedMaterial>
using MaterialRegister =
    Register<BambooMaterial, std::string, DerivedMaterial, const G4String &>;

#endif // BAMBOOMATERIALFACTORY_H
