#include "PandaXOpticalTrackingAction.hh"
#include "PandaXOpticalTrackInformation.hh"
#include "PandaXTrackInformation.hh"

#include <G4OpticalPhoton.hh>
#include <G4Track.hh>
#include <G4TrackingManager.hh>

void PandaXOpticalTrackingAction::PreUserTrackingAction(const G4Track *) {
}

void PandaXOpticalTrackingAction::PostUserTrackingAction(const G4Track *aTrack) {
    auto secondaries = fpTrackingManager->GimmeSecondaries();
    if (secondaries) {
        size_t nSeco = secondaries->size();
        for (size_t i = 0; i < nSeco; ++i) {
            auto track = (*secondaries)[i];
            if (track->GetParticleDefinition() ==
                G4OpticalPhoton::OpticalPhotonDefinition()) {
                auto info = new PandaXOpticalTrackInformation();
                info->SetParent(
                    aTrack->GetParticleDefinition()->GetParticleName());
                info->SetSourcePos(aTrack->GetPosition());
                track->SetUserInformation(info);
            } else {
		auto info = new PandaXTrackInformation();
		info->SetParent(aTrack->GetParticleDefinition()->GetParticleName());
		track->SetUserInformation(info);
            }
        }
    }
}
