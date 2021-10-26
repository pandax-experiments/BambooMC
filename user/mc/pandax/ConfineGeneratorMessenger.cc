#include "ConfineGeneratorMessenger.hh"
#include "ConfineGenerator.hh"

#include <G4SystemOfUnits.hh>

ConfineGeneratorMessenger::ConfineGeneratorMessenger(ConfineGenerator *gen)
    : myGen{gen}, loaderDir{new G4UIdirectory("/cgen/")},
      shapeCmd{new G4UIcmdWithAString("/cgen/shape", this)},
      confineCmd{new G4UIcmdWithAString("/cgen/confine", this)},
      centerCmd{new G4UIcmdWith3VectorAndUnit("/cgen/center", this)},
      radiusCmd{new G4UIcmdWithADoubleAndUnit("/cgen/radius", this)},
      heightCmd{new G4UIcmdWithADoubleAndUnit("/cgen/height", this)},
      upVecCmd{new G4UIcmdWith3Vector("/cgen/upvec", this)} {
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

void ConfineGeneratorMessenger::SetNewValue(G4UIcommand *command,
                                       G4String newValues) {
    if (command == shapeCmd.get()) {
        myGen->setShape(newValues);
    } else if (command == confineCmd.get()) {
        myGen->setConfineVolume(newValues);
    } else if (command == centerCmd.get()) {
        myGen->setCenter(centerCmd->GetNew3VectorValue(newValues));
    } else if (command == radiusCmd.get()) {
        myGen->setRadius(radiusCmd->GetNewDoubleValue(newValues));
    } else if (command == heightCmd.get()) {
        myGen->setHeight(heightCmd->GetNewDoubleValue(newValues));
    } else if (command == upVecCmd.get()) {
        myGen->setUpVec(upVecCmd->GetNew3VectorValue(newValues));
    } else {
        G4cout << "Wrong command" << G4endl;
    }
}
