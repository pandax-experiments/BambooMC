#include "SampleSteelContainer.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SampleSteelContainer, std::string, BambooParameters>
    SampleSteelContainer::reg("SampleSteelContainer");

SampleSteelContainer::SampleSteelContainer(const std::string &n,
                                           const BambooParameters &pars)
    : BambooDetector(n, pars) {
    G4cout << "create detector SampleSteelContainer..." << G4endl;
}

bool SampleSteelContainer::construct(const BambooParameters &,
                                     BambooDetector *parent) {
    // add construction code here
    if (parent == nullptr || parent->getContainerLV() == nullptr) {
        return false;
    }
    using namespace CLHEP;
    auto steel = G4Material::GetMaterial("G4_STAINLESS-STEEL");
    auto height = parameters.evaluateParameter("height");
    auto radius = parameters.evaluateParameter("radius");
    if (height == 0) {
        height = 2.1 * m;
    }
    if (radius == 0) {
        radius = 1.05 * m;
    }
    auto containerSolid =
        new G4Tubs("containerSoild", 0, radius, height / 2, 0, 2 * M_PI);
    mainLV =
        new G4LogicalVolume(containerSolid, steel, "ContainerLog", 0, 0, 0);
    mainPV = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), mainLV,
                               "SampleSteelContainer", parent->getContainerLV(),
                               false, 0);
    containerLV = mainLV;
    auto vis = new G4VisAttributes(G4Colour(0.89, 0.89, 0.89, 0.8));
    mainLV->SetVisAttributes(vis);

    return true;
}
