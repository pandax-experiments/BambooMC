#include "analysis/PandaXTrackingAction.hh"

#include <G4Track.hh>

PandaXTrackingAction::PandaXTrackingAction(shared_ptr<PandaXDataManager> dm)
    : _manager{dm} {}

PandaXTrackingAction::~PandaXTrackingAction() {}

void PandaXTrackingAction::PreUserTrackingAction(const G4Track *aTrack) {
    std::map<int, std::string> &trackMap = _manager->getTrackMap();
    int trackId = aTrack->GetTrackID();
    if (trackMap.find(trackId) == trackMap.end()) {
        trackMap[trackId] = aTrack->GetParticleDefinition()->GetParticleName();
    }
}

void PandaXTrackingAction::PostUserTrackingAction(const G4Track *) {}
