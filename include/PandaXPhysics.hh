#pragma once

#include "BambooPhysics.hh"
#include "BambooFactory.hh"

class PandaXPhysics : public BambooPhysics {
  public:
    PandaXPhysics(const BambooParameters &pars);

    ~PandaXPhysics() = default;

    static PhysicsRegister<PandaXPhysics> reg;
};

