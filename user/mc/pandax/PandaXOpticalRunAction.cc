#include "PandaXOpticalRunAction.hh"
#include "PandaXOpticalDataManager.hh"

#include <G4Run.hh>

#include <CLHEP/Random/Randomize.h>

#include <chrono>

PandaXOpticalRunAction::~PandaXOpticalRunAction() {
    G4cout << "PandaXOpticalRunAction destructed." << G4endl;
}

void PandaXOpticalRunAction::BeginOfRunAction(const G4Run *aRun) {
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    auto &dm = PandaXOpticalDataManager::getInstance();
    if (outputName == "")
        outputName = "bamboo_out.root";
    if (round == 0) {
        dm.book(outputName);
    } else {
        dm.book(outputName + "." + std::to_string(round));
    }
    dm.runId = aRun->GetRunID();
}

void PandaXOpticalRunAction::EndOfRunAction(const G4Run *aRun) {
    G4cout << "\n----------End of Run " << aRun->GetRunID() << "----------\n";
    PandaXOpticalDataManager::getInstance().save();
    PandaXOpticalDataManager::getInstance().close();
    ++round;
}
