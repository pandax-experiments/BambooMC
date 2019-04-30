#include "detector/SamplePolyHedra.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "analysis/PandaXSensitiveDetector.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4Polyhedra.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SamplePolyHedra> SamplePolyHedra::reg("SamplePolyHedra");

SamplePolyHedra::SamplePolyHedra(const G4String &name)
    : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SamplePolyHedra");
    _nSide = dp.getParameterAsInt("nSides");
    _innerRadius =
        BambooUtils::evaluate(dp.getParameterAsString("inner_radius"));
    _outerRadius =
        BambooUtils::evaluate(dp.getParameterAsString("outer_radius"));

    if (_nSide < 3) {
        _nSide = 3;
    }
    if (_innerRadius < 0) {
        _innerRadius = 0;
    }
    if (_outerRadius <= _innerRadius) {
        _outerRadius = _innerRadius + 0.5 * m;
    }
}

G4bool SamplePolyHedra::construct() {
    G4Material *liquidXenon = G4Material::GetMaterial("G4_lXe");
    double z[] = {0, 30 * cm};
    double rInner[2];
    double rOuter[2];
    for (int i = 0; i < 2; ++i) {
        rInner[i] = _innerRadius;
        rOuter[i] = _outerRadius;
    }
    G4Polyhedra *polyh = new G4Polyhedra("Polyhedra", 0 * degree, 360 * degree,
                                         _nSide, 2, z, rInner, rOuter);
    partLogicalVolume_ =
        new G4LogicalVolume(polyh, liquidXenon, "SamplePolyHedraLog", 0, 0, 0);
    partPhysicalVolume_ = new G4PVPlacement(
        0, G4ThreeVector(), partLogicalVolume_, "PolyheraDetector",
        parentPart_->getContainerLogicalVolume(), false, 0);
    partContainerLogicalVolume_ = partLogicalVolume_;

    // visibility attributes
    G4VisAttributes *sampleVisAtt = new G4VisAttributes(G4Colour(0., 0.6, 0.2));
    partLogicalVolume_->SetVisAttributes(sampleVisAtt);

    // add sensitive detector for energy deposition
    PandaXSensitiveDetector *sampleSD =
        new PandaXSensitiveDetector("SamplePolyhreaSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(sampleSD);
    partLogicalVolume_->SetSensitiveDetector(sampleSD);

    return true;
}
