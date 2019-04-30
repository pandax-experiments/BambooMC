#pragma once

#include "BambooPhysics.hh"
#include "BambooFactory.hh"

class PandaXOpticalPhysics : public BambooPhysics {
  public:
    PandaXOpticalPhysics(const BambooParameters &pars);

    ~PandaXOpticalPhysics() = default;

    static PhysicsRegister<PandaXOpticalPhysics> reg;
};

