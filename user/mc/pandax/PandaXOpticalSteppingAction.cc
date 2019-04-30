#include "PandaXOpticalSteppingAction.hh"
#include "PandaXOpticalSD.hh"

#include <G4Ions.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4ProcessManager.hh>
#include <G4SDManager.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

void PandaXOpticalSteppingAction::UserSteppingAction(const G4Step *aStep) {
    auto track = aStep->GetTrack();
    auto particleDef = track->GetDefinition();

    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    static G4ThreadLocal G4OpBoundaryProcess *boundary = nullptr;

    if (!boundary) {
        auto pm = track->GetDefinition()->GetProcessManager();
        auto nProcesses = pm->GetProcessListLength();
        auto pv = pm->GetProcessList();
        for (auto i = 0; i < nProcesses; ++i) {
            if ((*pv)[i]->GetProcessName() == "OpBoundary") {
                boundary = (G4OpBoundaryProcess *)(*pv)[i];
                break;
            }
        }
    }

    if (particleDef != G4OpticalPhoton::OpticalPhotonDefinition())
        return;

    auto postPoint = aStep->GetPostStepPoint();

    if (postPoint->GetStepStatus() != fGeomBoundary)
        return;

    boundaryStatus = boundary->GetStatus();

    switch (boundaryStatus) {
    case Detection: {
        auto sdManager = G4SDManager::GetSDMpointer();
        auto sd = (PandaXOpticalSD *)sdManager->FindSensitiveDetector(sdName);
        if (sd)
            sd->ProcessHits_constStep(aStep, nullptr);
        break;
    }
    default:
        break;
    }
}
