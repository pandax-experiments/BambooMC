#ifndef BAMBOOPHYSICSFACTORY_H
#define BAMBOOPHYSICSFACTORY_H

#include "BambooPhysics.hh"
#include "simple_factory.hh"

using BambooPhysicsFactory =
    SimpleFactory<BambooPhysics, std::string, const G4String &>;

template <typename DerivedPhysics>
using PhysicsRegister =
    Register<BambooPhysics, std::string, DerivedPhysics, const G4String &>;

#endif // BAMBOOPHYSICSFACTORY_H
