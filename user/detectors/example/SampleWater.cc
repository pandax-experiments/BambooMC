#include "SampleWater.hh"
#include "PandaXSensitiveDetector.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4SDParticleFilter.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SampleWater, std::string, BambooParameters>
    SampleWater::reg("SampleWater");

SampleWater::SampleWater(const std::string &n, const BambooParameters &pars)
    : BambooDetector(n, pars) {
    G4cout << "create detector SampleWater..." << G4endl;
}

bool SampleWater::construct(const BambooParameters &,
                            BambooDetector *parent) {
    // add construction code here
    if (parent == nullptr || parent->getContainerLV() == nullptr) {
        return false;
    }
    using namespace CLHEP;
    auto water = G4Material::GetMaterial("G4_WATER");
    auto width_x = parameters.evaluateParameter("width_x");
    auto width_y = parameters.evaluateParameter("width_y");
    auto width_z = parameters.evaluateParameter("width_z");
    if (width_x == 0) {
        width_x = 4 * m;
    }
    if (width_y == 0) {
        width_y = 4 * m;
    }
    if (width_z == 0) {
        width_z = 4 * m;
    }
    auto waterBox =
        new G4Box("WaterBox", width_x / 2, width_y / 2, width_z / 2);
    mainLV = new G4LogicalVolume(waterBox, water, "WaterLog", 0, 0, 0);
    containerLV = mainLV;
    auto vis = new G4VisAttributes(G4Colour(0.38, 0.83, 0.96, 0.8));
    mainLV->SetVisAttributes(vis);
    mainPV = new G4PVPlacement(0, G4ThreeVector(), mainLV, "SampleWater",
                               parent->getContainerLV(), false, 0);
    auto waterSD = new PandaXSensitiveDetector("WaterSD", false, true);
    auto sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(waterSD);
    mainLV->SetSensitiveDetector(waterSD);
    auto gammaFilter = new G4SDParticleFilter("gammaFilter");
    gammaFilter->add("gamma");
    waterSD->SetFilter(gammaFilter);
    return true;
}
