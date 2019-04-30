#include "EventLoaderMessenger.hh"
#include "EventLoader.hh"

#include <G4SystemOfUnits.hh>

EventLoaderMessenger::EventLoaderMessenger(EventLoader *loader)
    : myLoader{loader}, loaderDir{new G4UIdirectory("/evtloader/")},
      shapeCmd{new G4UIcmdWithAString("/evtloader/shape", this)},
      confineCmd{new G4UIcmdWithAString("/evtloader/confine", this)},
      centerCmd{new G4UIcmdWith3VectorAndUnit("/evtloader/center", this)},
      radiusCmd{new G4UIcmdWithADoubleAndUnit("/evtloader/radius", this)},
      heightCmd{new G4UIcmdWithADoubleAndUnit("/evtloader/height", this)},
      upVecCmd{new G4UIcmdWith3Vector("/evtloader/upvec", this)} {
    loaderDir->SetGuidance("event loader control commands.");

    shapeCmd->SetGuidance("Set shape of event loader.");
    shapeCmd->SetGuidance("Available shapes: point, cylinder");
    shapeCmd->SetParameterName("shape", true);
    shapeCmd->SetDefaultValue("point");
    shapeCmd->SetCandidates("point cylinder");

    confineCmd->SetGuidance(
        "Set confine of event loader (available for cylinder)");
    confineCmd->SetParameterName("confine", false);
    confineCmd->SetDefaultValue("");

    centerCmd->SetGuidance("Set center of the source point or cylinder");
    centerCmd->SetParameterName("x", "y", "z", true, true);
    centerCmd->SetDefaultUnit("cm");
    centerCmd->SetDefaultValue(G4ThreeVector(0, 0, 0));

    radiusCmd->SetGuidance("Set radius of the cylinder shape source");
    radiusCmd->SetParameterName("radius", false);
    radiusCmd->SetDefaultUnit("cm");
    radiusCmd->SetDefaultValue(0.5 * m);

    heightCmd->SetGuidance("Set height of the cylinder shape source");
    heightCmd->SetParameterName("height", false);
    heightCmd->SetDefaultUnit("cm");
    heightCmd->SetDefaultValue(1.0 * m);

    upVecCmd->SetGuidance("Set up vector of the cylinder shape source");
    upVecCmd->SetGuidance("example: 0 0 1");
    upVecCmd->SetParameterName("ux", "uy", "uz", false);
    upVecCmd->SetDefaultValue(G4ThreeVector(0, 0, 1));
}

void EventLoaderMessenger::SetNewValue(G4UIcommand *command,
                                       G4String newValues) {
    if (command == shapeCmd.get()) {
        myLoader->setShape(newValues);
    } else if (command == confineCmd.get()) {
        myLoader->setConfineVolume(newValues);
    } else if (command == centerCmd.get()) {
        myLoader->setCenter(centerCmd->GetNew3VectorValue(newValues));
    } else if (command == radiusCmd.get()) {
        myLoader->setRadius(radiusCmd->GetNewDoubleValue(newValues));
    } else if (command == heightCmd.get()) {
        myLoader->setHeight(heightCmd->GetNewDoubleValue(newValues));
    } else if (command == upVecCmd.get()) {
        myLoader->setUpVec(upVecCmd->GetNew3VectorValue(newValues));
    } else {
        G4cout << "Wrong command" << G4endl;
    }
}
