#pragma once

#include <G4VSensitiveDetector.hh>

#include "PandaXOpticalHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PandaXOpticalSD : public G4VSensitiveDetector {
public:
    PandaXOpticalSD(const G4String &name);

    ~PandaXOpticalSD() = default;

    virtual void Initialize(G4HCofThisEvent *);

    virtual void EndOfEvent(G4HCofThisEvent *);

    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *roHist);
    G4bool ProcessHits_constStep(const G4Step *aStep, G4TouchableHistory *roHist);

  private:
    PandaXOpticalHitsCollection *hitsCollection = nullptr;
};
