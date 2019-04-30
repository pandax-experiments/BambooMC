#include "detector/SampleWorldBox.hh"
#include "BambooGlobalVariables.hh"
#include "detector/BambooDetectorFactory.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SampleWorldBox> SampleWorldBox::reg("SampleWorldBox");

SampleWorldBox::SampleWorldBox(const G4String &name)
    : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SampleWorldBox");
    _halfX = dp.getParameterAsDouble("half_x");
    _halfY = dp.getParameterAsDouble("half_y");
    _halfZ = dp.getParameterAsDouble("half_z");
}

G4bool SampleWorldBox::construct() {
    G4Material *air = G4Material::GetMaterial("G4_AIR");
    G4Box *worldBox = new G4Box("WorldBox", _halfX, _halfY, _halfZ);
    partLogicalVolume_ =
        new G4LogicalVolume(worldBox, air, "WorldLog", 0, 0, 0);
    partPhysicalVolume_ = new G4PVPlacement(
        0, G4ThreeVector(), partLogicalVolume_, "World", 0, false, 0);
    partContainerLogicalVolume_ = partLogicalVolume_;
    G4VisAttributes *worldVis = new G4VisAttributes();
    worldVis->SetVisibility(false);
    partLogicalVolume_->SetVisAttributes(worldVis);
    return true;
}
