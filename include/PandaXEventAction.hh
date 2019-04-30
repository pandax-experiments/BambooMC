#pragma once

#include <G4UserEventAction.hh>


class G4Event;

class PandaXEventAction : public G4UserEventAction {
  public:
    PandaXEventAction() = default;

    virtual ~PandaXEventAction();

    virtual void BeginOfEventAction(const G4Event *aEvent);

    virtual void EndOfEventAction(const G4Event *aEvent);

    void setUserSeed(long seed) { user_seed = seed; }

  private:
    long user_seed = -1;
};


