#pragma once

#include <G4VSensitiveDetector.hh>

#include <map>
#include <string>

#include "PandaXEnergyDepositionHit.hh"
#include "PandaXSurfaceFluxHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4Box;

class PandaXSensitiveDetector : public G4VSensitiveDetector {
  public:
    PandaXSensitiveDetector(const G4String &name,
                            bool enableEnergyDeposition = true,
                            bool enableSurfaceFlux = false);

    ~PandaXSensitiveDetector() = default;

    virtual void Initialize(G4HCofThisEvent *);

    virtual void EndOfEvent(G4HCofThisEvent *);

  protected:
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *roHist);

  private:

    bool recordEnergyDeposition;
    bool recordSurfaceFlux;
    PandaXEnergyDepositionHitsCollection *eDHitsCollection = nullptr;
    PandaXSurfaceFluxHitsCollection *fsFluxHitsCollection = nullptr;
};
