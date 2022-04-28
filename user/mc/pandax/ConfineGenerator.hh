#pragma once

#include "BambooFactory.hh"
#include "BambooGenerator.hh"
#include "ConfineGeneratorMessenger.hh"

#include <G4ParticleDefinition.hh>

#include <set>
#include <vector>

class ConfineGeneratorMessenger;

class G4Event;

class ConfineGenerator : public BambooGenerator {
  public:
    ConfineGenerator(const BambooParameters &pars);

    ~ConfineGenerator() = default;

    virtual void GeneratePrimaries(G4Event *anEvent);

    static GeneratorRegister<ConfineGenerator> reg;

    void confineSourceToVolume(const G4String &v);

    void confineSourceToMaterial(const G4String &m);

    void setParticleType(const G4String &t);

    void setShape(const G4String &hShape) { shape = hShape; }

    void setCenter(const G4ThreeVector &hCenterCoords) {
        center = hCenterCoords;
    }

    void setRadius(G4double dRadius) { radius = dRadius; }

    void setHalfX(G4double dHalfx) { halfx = dHalfx; }

    void setHalfY(G4double dHalfy) { halfy = dHalfy; }

    void setHalfZ(G4double dHalfz) { halfz = dHalfz; }

    void setEnergy(G4double energy) { monoEnergy = energy; }

    void setDirection(const G4ThreeVector &d) { direction = d; }

    void setAngDistType(const G4String &type) { angular_type = type; }

    void setParticleDefinition(G4ParticleDefinition *def) {
        particle_definition = def;
    }

    G4double getParticleEnergy() const { return particle_energy; }

    const G4ThreeVector &getParticlePosition() { return particle_position; }

    const G4String &getShape() { return shape; }

  private:
    G4String confineVolume;
    G4ThreeVector generateRealPosition();
    G4ThreeVector generatePointsInVolume();
    G4bool isSourceConfined(const G4ThreeVector &pos);
    void generateIsotropicDirection();

    // class member variables
    std::unique_ptr<ConfineGeneratorMessenger> messenger;

    G4String particle_type;
    G4String shape;
    G4ThreeVector center = {0, 0, 0};
    G4double halfx;
    G4double halfy;
    G4double halfz;
    G4double radius;
    G4double monoEnergy;
    G4ThreeVector direction{0, 0, 1};
    G4String angular_type{};
    G4bool confine;
    G4bool confine_material;
    std::set<G4String> volume_names;

    G4ParticleDefinition *particle_definition;
    G4double particle_energy;
    G4ThreeVector particle_position;
};
