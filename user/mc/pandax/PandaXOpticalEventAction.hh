#pragma once

#include <G4UserEventAction.hh>


class G4Event;

class PandaXOpticalEventAction : public G4UserEventAction {
  public:
    PandaXOpticalEventAction() = default;

    virtual ~PandaXOpticalEventAction();

    virtual void BeginOfEventAction(const G4Event *aEvent);

    virtual void EndOfEventAction(const G4Event *aEvent);

};
