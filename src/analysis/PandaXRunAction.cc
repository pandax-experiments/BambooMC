#include "analysis/PandaXRunAction.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/PandaXDataManager.hh"

#include <G4Run.hh>

#include <CLHEP/Random/Randomize.h>

#include <chrono>

PandaXRunAction::PandaXRunAction(shared_ptr<PandaXDataManager> manager)
    : _dataManager{manager} {}

PandaXRunAction::~PandaXRunAction() {
    G4cout << "PandaXRunAction destructed." << G4endl;
}

void PandaXRunAction::BeginOfRunAction(const G4Run *aRun) {
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    const string &name = BambooGlobalVariables::Instance()->getOutDataName();
    _dataManager->book(name);
    _dataManager->_runId = aRun->GetRunID();
    auto now = std::chrono::system_clock::now();
    auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    now.time_since_epoch())
                    .count();
    CLHEP::HepRandom::setTheSeed(seed);
}

void PandaXRunAction::EndOfRunAction(const G4Run *aRun) {
    G4cout << "\n----------End of Run " << aRun->GetRunID() << "----------\n";
    _dataManager->save();
}
