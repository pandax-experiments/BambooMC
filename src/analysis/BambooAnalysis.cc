#include "analysis/BambooAnalysis.hh"

#include <G4UserEventAction.hh>
#include <G4UserRunAction.hh>
#include <G4UserStackingAction.hh>
#include <G4UserSteppingAction.hh>
#include <G4UserTrackingAction.hh>

BambooAnalysis::BambooAnalysis(const G4String &name)
    : _name(name), _runAction(0), _eventAction(0), _steppingAction(0),
      _stackingAction(0), _trackingAction(0) {}

G4UserRunAction *BambooAnalysis::getRunAction() { return _runAction; }

G4UserEventAction *BambooAnalysis::getEventAction() { return _eventAction; }

G4UserSteppingAction *BambooAnalysis::getSteppingAction() {
    return _steppingAction;
}

G4UserStackingAction *BambooAnalysis::getStackingAction() {
    return _stackingAction;
}

G4UserTrackingAction *BambooAnalysis::getTrackingAction() {
    return _trackingAction;
}
