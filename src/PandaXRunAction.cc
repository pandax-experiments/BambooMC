#include "PandaXRunAction.hh"
#include "PandaXDataManager.hh"

#include <G4Run.hh>

#include <CLHEP/Random/Randomize.h>

#include <chrono>

PandaXRunAction::~PandaXRunAction() {
    G4cout << "PandaXRunAction destructed." << G4endl;
}

void PandaXRunAction::BeginOfRunAction(const G4Run *aRun) {
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    auto &dm = PandaXDataManager::getInstance();
    if (outputName == "")
        outputName = "bamboo_out.root";
    if (round == 0) {
        dm.book(outputName);
    } else {
        dm.book(outputName + "." + std::to_string(round));
    }
    dm.runId = aRun->GetRunID();
    auto now = std::chrono::system_clock::now();
    auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now.time_since_epoch())
                    .count();
    CLHEP::HepRandom::setTheSeed(seed);
}

void PandaXRunAction::EndOfRunAction(const G4Run *aRun) {
    G4cout << "\n----------End of Run " << aRun->GetRunID() << "----------\n";
    PandaXDataManager::getInstance().save();
    PandaXDataManager::getInstance().close();
    ++round;
}
