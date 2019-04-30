#ifndef SIMPLEGPSGENERATOR_H
#define SIMPLEGPSGENERATOR_H

#include "generator/BambooGenerator.hh"
#include "generator/BambooGeneratorFactory.hh"

class G4GeneralParticleSource;
class G4Event;

class SimpleGPSGenerator : public BambooGenerator {
  public:
    SimpleGPSGenerator(const G4String &name);

    ~SimpleGPSGenerator();

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<SimpleGPSGenerator> reg;

  private:
    G4GeneralParticleSource *_particleGun;
};
#endif // SIMPLEGPSGENERATOR_H
