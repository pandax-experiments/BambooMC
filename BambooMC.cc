#include <G4RunManager.hh>

#include <G4VisExecutive.hh>

#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4GDMLParser.hh>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "BambooAnalysis.hh"
#include "BambooControl.hh"
#include "version.hh"

void usage(const char *exeName);

int main(int argc, char *argv[]) {

    G4cout << "Version: " << BAMBOOMC_VERSION << G4endl;

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
    auto physicsList = control.createPhysics();
    if (physicsList == nullptr) {
        std::cerr << "incorrect physics list name" << std::endl;
        return 1;
    }
    runManager->SetUserInitialization(physicsList);

    auto generator = control.createGenerator();
    if (generator == nullptr) {
        G4cerr << "incorrect generator name" << G4endl;
        return 1;
    }
    runManager->SetUserAction(generator);

    auto analysis = control.createAnalysis();
    if (analysis.get() == nullptr) {
        std::cerr << "no valid analysis provided." << std::endl;
    } else {

        auto runAction = analysis->getRunAction();
        auto eventAction = analysis->getEventAction();
        auto steppingAction = analysis->getSteppingAction();
        auto stackingAction = analysis->getStackingAction();
        auto trackingAction = analysis->getTrackingAction();

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
    }

    runManager->Initialize();
    G4cout << "Run manager initialization done!" << G4endl;
    if (control.getGdmlFileName() != "") {
        auto world = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();
        G4GDMLParser parser;
#if G4VERSION_NUMBER>1030
        parser.SetSDExport(true);
#endif
        parser.Write(control.getGdmlFileName(), world, false);
        return 0;
    }
    auto visManager = std::make_unique<G4VisExecutive>();
    visManager->Initialize();
    G4cout << "Visualization manager initialization done!" << G4endl;
    G4UImanager *uiManager = G4UImanager::GetUIpointer();

    if (!control.getMacFileName().empty()) {
        G4String command = "/control/execute " + control.getMacFileName();
        G4cout << command << G4endl;
        uiManager->ApplyCommand(command);
    }

    if (!ui) {
        G4String command = "/run/beamOn " + std::to_string(control.nEvents());
        uiManager->ApplyCommand(command);
    } else {
        ui->SessionStart();
    }

    return 0;
}

void usage(const char *exeName) {
    std::cout << "Usage: " << exeName << " [macro]" << std::endl;
}
