#pragma once

#include <G4UserTrackingAction.hh>

class G4Track;

class PandaXOpticalTrackingAction : public G4UserTrackingAction {
  public:
    PandaXOpticalTrackingAction() = default;

    ~PandaXOpticalTrackingAction() = default;

    virtual void PreUserTrackingAction(const G4Track *aTrack);
    virtual void PostUserTrackingAction(const G4Track *aTrack);

  private:
};
