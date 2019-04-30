#include <G4RunManager.hh>
#include <G4String.hh>

#include <G4PhysListFactory.hh>
#include <QBBC.hh>

#include <G4VisExecutive.hh>

#include <G4UIExecutive.hh>
#include <G4UImanager.hh>

#include <iostream>
#include <sstream>
#include <string>

#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "generator/BambooGeneratorFactory.hh"
#include "physics/BambooPhysicsFactory.hh"

void usage(const char *exeName);

int main(int argc, char *argv[]) {
    BambooControl::getControl()->setup(argc, argv);

    G4cout << "Control loaded." << G4endl;

    G4UIExecutive *ui = 0;
    if (BambooControl::INTERACTIVE) {
        ui = new G4UIExecutive(argc, argv);
    }
    auto runManager = new G4RunManager;

    runManager->SetRunIDCounter(
        BambooGlobalVariables::Instance()->getRunNumber());
    runManager->SetUserInitialization(new BambooDetectorConstruction);

    if (BambooGlobalVariables::Instance()->getPhysicsName().find("Physics") ==
        std::string::npos) {
        auto physListFactory = new G4PhysListFactory();
        G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList(
            BambooGlobalVariables::Instance()->getPhysicsName());
        runManager->SetUserInitialization(physicsList);
    } else {
        auto physicsList = BambooPhysicsFactory::create(
            BambooGlobalVariables::Instance()->getPhysicsName(),
            BambooGlobalVariables::Instance()->getPhysicsName());
        runManager->SetUserInitialization(
            (G4VModularPhysicsList *)physicsList.release());
    }

    auto generator = BambooGeneratorFactory::create(
        BambooGlobalVariables::Instance()->getGeneratorName(),
        BambooGlobalVariables::Instance()->getGeneratorName());

    runManager->SetUserAction(
        (G4VUserPrimaryGeneratorAction *)generator.release());

    // analysis
    auto analysis = BambooAnalysisFactory::create(
        BambooGlobalVariables::Instance()->getAnalysisName(),
        BambooGlobalVariables::Instance()->getAnalysisName());

    G4UserRunAction *runAction = analysis->getRunAction();
    G4UserEventAction *eventAction = analysis->getEventAction();
    G4UserSteppingAction *steppingAction = analysis->getSteppingAction();
    G4UserStackingAction *stackingAction = analysis->getStackingAction();
    G4UserTrackingAction *trackingAction = analysis->getTrackingAction();

    if (runAction != nullptr) {
        runManager->SetUserAction(runAction);
    }
    if (eventAction != nullptr) {
        runManager->SetUserAction(eventAction);
    }
    if (steppingAction != nullptr) {
        runManager->SetUserAction(steppingAction);
    }
    if (stackingAction != nullptr) {
        runManager->SetUserAction(stackingAction);
    }
    if (trackingAction != nullptr) {
        runManager->SetUserAction(trackingAction);
    }

    runManager->Initialize();
    G4cout << "Run manager initialization done!" << G4endl;
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();
    G4cout << "Visualization manager initialization done!" << G4endl;
    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    if (!BambooControl::MacroFileName.isNull()) {
        G4String command = "/control/execute " + BambooControl::MacroFileName;
        G4cout << command << G4endl;
        uiManager->ApplyCommand(command);
    }

    if (BambooControl::NUM_EVENTS) {
        std::ostringstream ss;
        ss << BambooControl::NUM_EVENTS;
        G4String command = "/run/beamOn " + ss.str();
        G4cout << command << G4endl;
        uiManager->ApplyCommand(command);
    }

    if (ui) {
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    G4cout << "G4RunManager deleted." << G4endl;
    return 0;
}

void usage(const char *exeName) {
    std::cout << "Usage: " << exeName << " [macro]" << std::endl;
}
