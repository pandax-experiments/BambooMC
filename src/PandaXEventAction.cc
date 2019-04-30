#include "PandaXEventAction.hh"
#include "PandaXDataManager.hh"

#include <CLHEP/Random/Random.h>

#include <G4Event.hh>

#include <chrono>

PandaXEventAction::~PandaXEventAction() {
    G4cout << "PandaXEventAction destructed." << G4endl;
}

void PandaXEventAction::BeginOfEventAction(const G4Event *aEvent) {
    int eventId = aEvent->GetEventID();
    auto &dm = PandaXDataManager::getInstance();
    dm.eventId = eventId;
    dm.partialSaved = false;
    if (eventId % 100000 == 0) {
        G4cout << "Processing event " << eventId << "..." << G4endl;
    }
    if (user_seed != -1) {
        CLHEP::HepRandom::setTheSeed(user_seed);
        dm.seed = user_seed;
    } else {
        // reset the random seed
        auto now = std::chrono::system_clock::now();
        auto generator = CLHEP::HepRandom::getTheGenerator();
        auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        now.time_since_epoch())
                        .count() %
                    static_cast<int64_t>(1e16 * generator->flat());
        CLHEP::HepRandom::setTheSeed(seed);
        dm.seed = seed;
    }
}

void PandaXEventAction::EndOfEventAction(const G4Event *aEvent) {
    PandaXDataManager::getInstance().fillEvent(aEvent);
}
