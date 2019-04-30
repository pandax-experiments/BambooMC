#ifndef SIMPLEUNDERGROUNDPHYSICS_H
#define SIMPLEUNDERGROUNDPHYSICS_H

#include "physics/BambooPhysics.hh"
#include "physics/BambooPhysicsFactory.hh"

class SimpleUnderGroundPhysics : public BambooPhysics {
  public:
    SimpleUnderGroundPhysics(const G4String &name);

    ~SimpleUnderGroundPhysics();

    static PhysicsRegister<SimpleUnderGroundPhysics> reg;
};
#endif // SIMPLEUNDERGROUNDPHYSICS
