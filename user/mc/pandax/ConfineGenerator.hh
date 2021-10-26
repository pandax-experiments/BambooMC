#pragma once

#include "BambooFactory.hh"
#include "BambooGenerator.hh"

class ConfineGenerator : public BambooGenerator {
  public:
    ConfineGenerator(const BambooParameters &pars);

    ~ConfineGenerator() = default;

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<ConfineGenerator> reg;

  private:
};
