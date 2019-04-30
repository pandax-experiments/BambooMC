#include "PandaXOpticalEventAction.hh"
#include "PandaXOpticalDataManager.hh"

#include <G4Event.hh>

PandaXOpticalEventAction::~PandaXOpticalEventAction() {
    G4cout << "PandaXOpticalEventAction destructed." << G4endl;
}

void PandaXOpticalEventAction::BeginOfEventAction(const G4Event *aEvent) {
    int eventId = aEvent->GetEventID();
    auto &dm = PandaXOpticalDataManager::getInstance();
    dm.eventId = eventId;
    if (eventId % 100000 == 0) {
        G4cout << "Processing event " << eventId << "..." << G4endl;
    }
}

void PandaXOpticalEventAction::EndOfEventAction(const G4Event *aEvent) {
    PandaXOpticalDataManager::getInstance().fillEvent(aEvent);
}
