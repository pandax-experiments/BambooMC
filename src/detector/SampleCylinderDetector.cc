#include "detector/SampleCylinderDetector.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/PandaXSensitiveDetector.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>

#include <G4VisAttributes.hh>

#include <math.h>

DetectorRegister<SampleCylinderDetector>
    SampleCylinderDetector::reg("SampleCylinderDetector");

SampleCylinderDetector::SampleCylinderDetector(const G4String &name)
    : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SampleCylinderDetector");
    _radius = dp.getParameterAsDouble("radius") * mm;
    _height = dp.getParameterAsDouble("height") * mm;
    _shiftX = dp.getParameterAsDouble("shiftX") * mm;
    _shiftY = dp.getParameterAsDouble("shiftY") * mm;
    _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
    G4cout << "radius: " << _radius / mm << "mm." << G4endl;
}

G4bool SampleCylinderDetector::construct() {
    G4Material *liquidXenon = G4Material::GetMaterial("G4_lXe");
    G4Tubs *sampleDetectorTub = new G4Tubs("SampleDetectorTub", 0, _radius,
                                           _height / 2., 0., 2. * M_PI);
    partLogicalVolume_ = new G4LogicalVolume(sampleDetectorTub, liquidXenon,
                                             "SampleDetectorLog", 0, 0, 0);
    partPhysicalVolume_ = new G4PVPlacement(
        0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), partLogicalVolume_,
        "SampleDetector", parentPart_->getContainerLogicalVolume(), false, 0);
    partContainerLogicalVolume_ = partLogicalVolume_;
    G4VisAttributes *sampleDetectorVisAtt =
        new G4VisAttributes(G4Colour(0., 0.6, 0.1));
    partLogicalVolume_->SetVisAttributes(sampleDetectorVisAtt);
    PandaXSensitiveDetector *cylinderSD =
        new PandaXSensitiveDetector("SampleCylinderSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(cylinderSD);
    partLogicalVolume_->SetSensitiveDetector(cylinderSD);
    G4cout << "Sensitive Mass: " << partLogicalVolume_->GetMass() / kg << " kg."
           << G4endl;
    return true;
}
