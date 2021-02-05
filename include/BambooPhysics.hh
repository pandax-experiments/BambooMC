#pragma once

#include <G4VModularPhysicsList.hh>

#include "BambooControl.hh"

class BambooPhysics : public G4VModularPhysicsList {
  public:
    BambooPhysics(const BambooParameters &pars) : physicsParameters(pars) {}
    virtual ~BambooPhysics() = default;

  protected:
    BambooParameters physicsParameters;
};
