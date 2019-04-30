#pragma once

#include <G4UserStackingAction.hh>

class G4Track;

class PandaXStackingAction : public G4UserStackingAction {
  public:
    PandaXStackingAction(double v) : chainSplittingLifeTime{v} {}

    ~PandaXStackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *aTrack);

    virtual void NewStage();

    virtual void PrepareNewEvent();

  private:

    double chainSplittingLifeTime;
};
