#pragma once

#include "BambooControl.hh"

class G4UserRunAction;
class G4UserEventAction;
class G4UserSteppingAction;
class G4UserTrackingAction;
class G4UserStackingAction;

class BambooAnalysis {
  public:
    BambooAnalysis(const BambooParameters &pars) : analysisParameters(pars) {}

    virtual ~BambooAnalysis() {}

    G4UserRunAction *getRunAction() { return runAction; }

    G4UserEventAction *getEventAction() { return eventAction; }

    G4UserSteppingAction *getSteppingAction() { return steppingAction; }

    G4UserStackingAction *getStackingAction() { return stackingAction; }

    G4UserTrackingAction *getTrackingAction() { return trackingAction; }

  protected:
    BambooParameters analysisParameters;

    G4UserRunAction *runAction = nullptr;

    G4UserEventAction *eventAction = nullptr;

    G4UserSteppingAction *steppingAction = nullptr;

    G4UserStackingAction *stackingAction = nullptr;

    G4UserTrackingAction *trackingAction = nullptr;
};
