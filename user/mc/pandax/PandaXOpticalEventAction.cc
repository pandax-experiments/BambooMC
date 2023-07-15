#include "PandaXOpticalEventAction.hh"
#include "PandaXOpticalDataManager.hh"

#include <CLHEP/Random/Random.h>

#include <G4Event.hh>

#include <chrono>

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
    auto now = std::chrono::system_clock::now();
    auto generator = CLHEP::HepRandom::getTheGenerator();
    auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now.time_since_epoch())
                    .count() %
                static_cast<int64_t>(1e16 * generator->flat());
    CLHEP::HepRandom::setTheSeed(seed);
}

void PandaXOpticalEventAction::EndOfEventAction(const G4Event *aEvent) {
    PandaXOpticalDataManager::getInstance().fillEvent(aEvent);
}
