#include "SampleCylinder.hh"
#include "PandaXSensitiveDetector.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SampleCylinder, std::string, BambooParameters>
    SampleCylinder::reg("SampleCylinder");

SampleCylinder::SampleCylinder(const std::string &n,
                               const BambooParameters &pars)
    : BambooDetector(n, pars) {
    G4cout << "create detector SampleCylinder..." << G4endl;
}

bool SampleCylinder::construct(const BambooParameters &,
                               BambooDetector *parent) {
    // add construction code here
    if (parent == nullptr || parent->getContainerLV() == nullptr) {
        return false;
    }
    using namespace CLHEP;
    auto lXe = G4Material::GetMaterial("G4_lXe");
    auto height = parameters.evaluateParameter("height");
    auto radius = parameters.evaluateParameter("radius");
    auto shift_x = parameters.evaluateParameter("shift_x");
    auto shift_y = parameters.evaluateParameter("shift_y");
    auto shift_z = parameters.evaluateParameter("shift_z");
    if (height == 0) {
        height = 2 * m;
    }
    if (radius == 0) {
        radius = 1 * m;
    }
    auto detectorSoild =
        new G4Tubs("detectorSoild", 0, radius, height / 2, 0, 2 * M_PI);
    mainLV =
        new G4LogicalVolume(detectorSoild, lXe, "SampleCylinderLV", 0, 0, 0);
    mainPV =
        new G4PVPlacement(0, G4ThreeVector(shift_x, shift_y, shift_z), mainLV,
                          "SampleCylinder", parent->getContainerLV(), false, 0);
    containerLV = mainLV;
    auto vis = new G4VisAttributes(G4Colour(0.9, 1.0, 0.9, 0.8));
    mainLV->SetVisAttributes(vis);
    auto sd = new PandaXSensitiveDetector("CylinderSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    mainLV->SetSensitiveDetector(sd);
    return true;
}
