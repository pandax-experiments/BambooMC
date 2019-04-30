#pragma once

#include "BambooFactory.hh"
#include "BambooGenerator.hh"

#include <G4HEPEvtInterface.hh>
#include <memory>

class G4Event;

class HEPEvtLoader : public BambooGenerator {
  public:
    HEPEvtLoader(const BambooParameters &pars);

    ~HEPEvtLoader() = default;

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<HEPEvtLoader> reg;

  private:
    std::unique_ptr<G4HEPEvtInterface> evt; 
};
