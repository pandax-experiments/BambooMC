#include "PandaXTrackingAction.hh"
#include "PandaXTrackInformation.hh"

#include <G4Track.hh>
#include <G4TrackingManager.hh>

void PandaXTrackingAction::PreUserTrackingAction(const G4Track *) {}

void PandaXTrackingAction::PostUserTrackingAction(const G4Track *aTrack) {
    auto secondaries = fpTrackingManager->GimmeSecondaries();
    if (secondaries) {
        size_t nSeco = secondaries->size();
        for (size_t i = 0; i < nSeco; ++i) {
            auto info = new PandaXTrackInformation();
            info->SetParent(aTrack->GetParticleDefinition()->GetParticleName());
            (*secondaries)[i]->SetUserInformation(info);
        }
    }
}
