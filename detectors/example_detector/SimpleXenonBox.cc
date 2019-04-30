#include "SimpleXenonBox.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "detector/BambooDetectorFactory.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4SDParticleFilter.hh>
#include <G4ThreeVector.hh>
#include <G4VSDFilter.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SimpleXenonBox> SimpleXenonBox::reg("SimpleXenonBox");

SimpleXenonBox::SimpleXenonBox(const G4String &name)
    : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SimpleXenonBox");

    halfx_ = BambooUtils::evaluate(dp.getParameterAsString("halfx"));
    halfy_ = BambooUtils::evaluate(dp.getParameterAsString("halfy"));
    halfz_ = BambooUtils::evaluate(dp.getParameterAsString("halfz"));
    G4cout << "SimpleXenonBox with name " << name << " constructed." << G4endl;
}

G4bool SimpleXenonBox::construct() {
    G4Material *lxe = G4Material::GetMaterial("G4_lXe");
    G4Box *lxeSolid = new G4Box("LXeSolid", halfx_, halfy_, halfz_);
    partLogicalVolume_ = new G4LogicalVolume(lxeSolid, lxe, "lxeLog", 0, 0, 0);
    G4VisAttributes *lxe_vis =
        new G4VisAttributes(G4Colour(0.1, 0.9, 0.3, 0.5));
    partLogicalVolume_->SetVisAttributes(lxe_vis);
    partPhysicalVolume_ = new G4PVPlacement(
        0, G4ThreeVector(), partLogicalVolume_, "SimpleXenonBox",
        parentPart_->getContainerLogicalVolume(), false, 0);

    // sensitivity detector
    PandaXSensitiveDetector *simpleSD =
        new PandaXSensitiveDetector("SimpleXenonSD", false, true);
    G4SDManager::GetSDMpointer()->AddNewDetector(simpleSD);
    partLogicalVolume_->SetSensitiveDetector(simpleSD);
    G4SDParticleFilter *pFilter = new G4SDParticleFilter("pFilter");
    pFilter->add("gamma");
    pFilter->add("neutron");
    simpleSD->SetFilter(pFilter);

    partContainerLogicalVolume_ = partLogicalVolume_;
    return true;
}
