#include "PandaXSteppingAction.hh"

#include <G4Ions.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4VProcess.hh>
#include <G4Version.hh>

PandaXSteppingAction::~PandaXSteppingAction() {}

void PandaXSteppingAction::UserSteppingAction(const G4Step *aStep) {
    G4Track *aTrack = aStep->GetTrack();
    auto *def = aTrack->GetParticleDefinition();
    if (def->GetParticleType() != "nucleus" || aTrack->GetTrackID() == 1) {
        return;
    }
    if (aTrack->GetTrackStatus() != fStopButAlive)
        return;
    if (def->GetIonLifeTime() > chainSplittingLifeTime) {
        aTrack->SetTrackStatus(fPostponeToNextEvent);
        aTrack->SetGlobalTime(0.0 * second);
    }
}
