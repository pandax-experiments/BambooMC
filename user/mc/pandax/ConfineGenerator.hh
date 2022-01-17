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
    void setShape(const G4String & v);
    void setConfineVolume(const G4String &v);
    void setConfineMaterial(const G4String &v);
    void setParticle(const G4String &v);
    void setCenter(const G4ThreeVector &v);
    void setRadius(G4double v);
    void setHalfX(G4double v);
    void setHalfY(G4double v);
    void setHalfZ(G4double v);
    void setEnergy(G4double v);
};
