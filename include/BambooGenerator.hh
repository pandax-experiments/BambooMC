#pragma once

#include <G4VUserPrimaryGeneratorAction.hh>

class BambooGenerator : public G4VUserPrimaryGeneratorAction {
  public:
    BambooGenerator(const BambooParameters &pars) : generatorParameters(pars) {};

    virtual ~BambooGenerator() = default;

  protected:
    const BambooParameters &generatorParameters;
};

