#pragma once

#include <G4UserTrackingAction.hh>

class G4Track;

class PandaXTrackingAction : public G4UserTrackingAction {
  public:
    PandaXTrackingAction() = default;

    ~PandaXTrackingAction() = default;

    virtual void PreUserTrackingAction(const G4Track *aTrack);
    virtual void PostUserTrackingAction(const G4Track *aTrack);
};
