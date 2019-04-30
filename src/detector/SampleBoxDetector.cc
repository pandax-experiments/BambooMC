#include "detector/SampleBoxDetector.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "analysis/PandaXSensitiveDetector.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SampleBoxDetector> SampleBoxDetector::reg("SampleBoxDetector");

SampleBoxDetector::SampleBoxDetector(const G4String &name)
    : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SampleBoxDetector");
    _halfX = BambooUtils::evaluate(dp.getParameterAsString("half_x"));
    _halfY = BambooUtils::evaluate(dp.getParameterAsString("half_y"));
    _halfZ = BambooUtils::evaluate(dp.getParameterAsString("half_z"));

    if (_halfX == 0) {
        _halfX = 1.0 * m;
    }
    if (_halfY == 0) {
        _halfY = 1.0 * m;
    }
    if (_halfZ == 0) {
        _halfZ = 1.0 * m;
    }

    G4cout << "SampleBoxDetector found..." << G4endl;
}

G4bool SampleBoxDetector::construct() {
    G4Material *liquidXenon = G4Material::GetMaterial("G4_lXe");
    G4Box *sampleDetectorBox =
        new G4Box("SampleDetectorBox", _halfX, _halfY, _halfZ);
    partLogicalVolume_ = new G4LogicalVolume(sampleDetectorBox, liquidXenon,
                                             "SampleDetectorLog", 0, 0, 0);
    partPhysicalVolume_ = new G4PVPlacement(
        0, G4ThreeVector(), partLogicalVolume_, "SampleDetector",
        parentPart_->getContainerLogicalVolume(), false, 0);
    partContainerLogicalVolume_ = partLogicalVolume_;

    // visibility attributes
    G4VisAttributes *sampleBoxVisAtt =
        new G4VisAttributes(G4Colour(0., 0.6, 0.2));
    partLogicalVolume_->SetVisAttributes(sampleBoxVisAtt);

    // add sensitive detector for energy deposition
    PandaXSensitiveDetector *sampleBoxSD =
        new PandaXSensitiveDetector("SampleBoxSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(sampleBoxSD);
    partLogicalVolume_->SetSensitiveDetector(sampleBoxSD);

    return true;
}
