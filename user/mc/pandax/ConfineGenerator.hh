#pragma once

#include <G4Navigator.hh>
#include <G4ParticleDefinition.hh>
#include "BambooFactory.hh"
#include "BambooGenerator.hh"
#include "ConfineGeneratorMessenger.hh"
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

    void setShape(const G4String &hShape) { m_hShape = hShape; }

    void ConfineSourceToVolume(G4String);

    void ConfineSourceToMaterial(G4String);

    void setParticle(const G4String &v) { Particle = v; }

    void setCenter(const G4ThreeVector &hCenterCoords) {
        m_hCenterCoords = hCenterCoords;
    }

    void setRadius(G4double dRadius) { m_dRadius = dRadius; }

    void setHalfX(G4double dHalfx) { m_dHalfx = dHalfx; }

    void setHalfY(G4double dHalfy) { m_dHalfy = dHalfy; }

    void setHalfZ(G4double dHalfz) { m_dHalfz = dHalfz; }

    void setEnergy(G4double energy) { m_dMonoEnergy = energy; }

    G4double GetParticleEnergy() const { return m_dParticleEnergy; }

    const G4ThreeVector &GetParticlePosition() { return m_hParticlePosition; }

    const G4String &getShape() { return m_hShape; }

    void SetParticleDefinition(G4ParticleDefinition *pParticleDefinition);

    inline void SetParticleCharge(G4double dCharge) {
        m_dParticleCharge = dCharge;
    }


  private:
    G4String confineVolume;
    G4ThreeVector GenerateRealPosition();
    G4ThreeVector GeneratePointsInVolume();
    G4bool IsSourceConfined(const G4ThreeVector &pos);
    std::unique_ptr<ConfineGeneratorMessenger> messenger;
    G4Navigator *m_pNavigator;

    G4String Particle;

    G4String m_hShape;
    G4ThreeVector m_hCenterCoords;
    G4double m_dHalfx;
    G4double m_dHalfy;
    G4double m_dHalfz;
    G4double m_dRadius;
    G4double m_dMonoEnergy;
    G4bool m_bConfine;
    G4bool m_bConfine_Material;
    std::set<G4String> m_hVolumeNames;
    std::set<G4String> m_hMaterialNames;

    std::vector<G4ParticleDefinition *> particleDefinitionVec;
    G4double m_dParticleEnergy;
    G4double m_dParticleCharge;
    G4ThreeVector m_hParticlePosition;

};
