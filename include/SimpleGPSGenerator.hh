#pragma once

#include "BambooFactory.hh"
#include "BambooGenerator.hh"

#include <memory>

class G4GeneralParticleSource;
class G4Event;

class SimpleGPSGenerator : public BambooGenerator {
  public:
    SimpleGPSGenerator(const BambooParameters &pars);

    ~SimpleGPSGenerator() = default;

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<SimpleGPSGenerator> reg;

  private:
    std::unique_ptr<G4GeneralParticleSource> gun;
};
