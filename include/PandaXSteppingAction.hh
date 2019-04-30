#pragma once

#include <G4UserSteppingAction.hh>

class G4Step;

class PandaXSteppingAction : public G4UserSteppingAction {
  public:
    PandaXSteppingAction(double v) : chainSplittingLifeTime{v} {};

    ~PandaXSteppingAction();

    virtual void UserSteppingAction(const G4Step *aStep);

  private:
    double chainSplittingLifeTime;
};
