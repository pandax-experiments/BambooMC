#ifndef PANDAXSENSITIVEDETECTOR_H
#define PANDAXSENSITIVEDETECTOR_H

#include <G4VSensitiveDetector.hh>

#include <map>
#include <string>

#include "analysis/PandaXEnergyDepositionHit.hh"
#include "analysis/PandaXFlatSurfaceFluxHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4Box;

class PandaXSensitiveDetector : public G4VSensitiveDetector {
  public:
    PandaXSensitiveDetector(const G4String &name,
                            bool enableEnergyDeposition = true,
                            bool enableFlatSurfaceFlux = false);

    ~PandaXSensitiveDetector();

    virtual void Initialize(G4HCofThisEvent *);

    virtual void EndOfEvent(G4HCofThisEvent *);

  protected:
    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *roHist);

  private:
    int determineBoxSurface(G4Step *aStep, G4Box *boxSolid);

    bool _recordEnergyDeposition;
    bool _recordFlatSurfaceFlux;
    PandaXEnergyDepositionHitsCollection *_eDHitsCollection;
    PandaXFlatSurfaceFluxHitsCollection *_fsFluxHitsCollection;
};

#endif // PANDAXSENSITIVEDETECTOR_H
