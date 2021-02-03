#include <G4RunManager.hh>
// #include <G4String.hh>

// #include <G4PhysListFactory.hh>
// #include <QBBC.hh>

// #include <G4VisExecutive.hh>

#include <G4UIExecutive.hh>
#include <G4UImanager.hh>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "BambooControl.hh"

/*
#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "generator/BambooGeneratorFactory.hh"
#include "physics/BambooPhysicsFactory.hh"
*/
void usage(const char *exeName);

int main(int argc, char *argv[]) {

    BambooControl control;

    control.setup(argc, argv);
    G4cout << "Control loaded." << G4endl;

    control.print();

    std::unique_ptr<G4UIExecutive> ui{nullptr};
    if (control.isInterative()) {
        ui = std::make_unique<G4UIExecutive>(argc, argv);
    }

    std::unique_ptr<G4RunManager> runManager{new G4RunManager};

    runManager->SetRunIDCounter(control.getRunNumber());

    // // initialize the detector
    runManager->SetUserInitialization(control.createDetector());

    // // initialize the physics
    // auto physicsName = control.getGlobalVariables().getPhysicsName();
    // if (physicsName.find("Physics") == std::string::npos) {
    //     auto physListFactory = new G4PhysListFactory();
    //     G4VUserPhysicsList *physicsList =
    //         physListFactory->GetReferencePhysList(physicsName);
    //     if (physicsList == 0) {
    //         G4cerr << "The provided physics list [" << physicsName
    //                << "] is not found." << G4endl;
    //         return 1;
    //     }
    //     runManager->SetUserInitialization(physicsList);
    // } else {
    //     auto physicsList =
    //         BambooPhysicsFactory::create(physicsName, physicsName);
    //     runManager->SetUserInitialization(
    //         (G4VModularPhysicsList *)physicsList.release());
    // }

    // auto generator = BambooGeneratorFactory::create(
    //     control.getGlobalVariables().getGeneratorName(),
    //     control.getGlobalVariables().getGeneratorName());

    // runManager->SetUserAction(
    //     (G4VUserPrimaryGeneratorAction *)generator.release());

    // // analysis
    // auto analysis = BambooAnalysisFactory::create(
    //     control.getGlobalVariables().getAnalysisName(),
    //     control.getGlobalVariables().getAnalysisName());

    // G4UserRunAction *runAction = analysis->getRunAction();
    // G4UserEventAction *eventAction = analysis->getEventAction();
    // G4UserSteppingAction *steppingAction = analysis->getSteppingAction();
    // G4UserStackingAction *stackingAction = analysis->getStackingAction();
    // G4UserTrackingAction *trackingAction = analysis->getTrackingAction();

    // if (runAction != nullptr) {
    //     runManager->SetUserAction(runAction);
    // }
    // if (eventAction != nullptr) {
    //     runManager->SetUserAction(eventAction);
    // }
    // if (steppingAction != nullptr) {
    //     runManager->SetUserAction(steppingAction);
    // }
    // if (stackingAction != nullptr) {
    //     runManager->SetUserAction(stackingAction);
    // }
    // if (trackingAction != nullptr) {
    //     runManager->SetUserAction(trackingAction);
    // }

    // runManager->Initialize();
    // G4cout << "Run manager initialization done!" << G4endl;
    // G4VisManager *visManager = new G4VisExecutive;
    // visManager->Initialize();
    // G4cout << "Visualization manager initialization done!" << G4endl;
    // G4UImanager *uiManager = G4UImanager::GetUIpointer();

    // if (!control.getMacFileName().empty()) {
    //     G4String command = "/control/execute " + control.getMacFileName();
    //     G4cout << command << G4endl;
    //     uiManager->ApplyCommand(command);
    // }

    // if (!ui) {
    //     std::ostringstream ss;
    //     ss << control.nEvents();
    //     G4String command = "/run/beamOn " + ss.str();
    //     G4cout << command << G4endl;
    //     uiManager->ApplyCommand(command);
    // }

    // if (ui) {
    //     ui->SessionStart();
    // }

    // delete visManager;
    return 0;
}

void usage(const char *exeName) {
    std::cout << "Usage: " << exeName << " [macro]" << std::endl;
}
