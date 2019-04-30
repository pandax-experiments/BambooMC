#include "analysis/PandaXSteppingAction.hh"

#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4VProcess.hh>
#include <G4Version.hh>

PandaXSteppingAction::PandaXSteppingAction(shared_ptr<PandaXDataManager> dm)
    : _manager{dm} {}

PandaXSteppingAction::~PandaXSteppingAction() {}

void PandaXSteppingAction::UserSteppingAction(const G4Step *aStep) {
    G4Track *aTrack = aStep->GetTrack();
    const G4ParticleDefinition *def = aTrack->GetParticleDefinition();

// the nucleus at ground state with 0 kinetic energy should be marked as
// postpone, they always come from alpha decay, time reset here
#if G4VERSION_NUMBER < 1020
    if (def->GetParticleType() == "nucleus" &&
        aTrack->GetTrackStatus() == fStopButAlive &&
        def->GetParticleName().contains("[0.0]")) {
#else // G4VERSION_NUMBER > 1020
    if (def->GetParticleType() == "nucleus" &&
        aTrack->GetTrackStatus() == fStopButAlive &&
        !def->GetParticleName().contains("[") &&
        !def->GetParticleName().contains("alpha")) {
#endif
        aTrack->SetTrackStatus(fPostponeToNextEvent);
        aTrack->SetGlobalTime(0.0 * second);
    }
}
