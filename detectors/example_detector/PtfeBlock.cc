#include "PtfeBlock.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "detector/BambooDetectorFactory.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<PtfeBlock> PtfeBlock::reg("PtfeBlock");

PtfeBlock::PtfeBlock(const G4String &name) : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "PtfeBlock");

    halfx_ = BambooUtils::evaluate(dp.getParameterAsString("halfx"));
    halfy_ = BambooUtils::evaluate(dp.getParameterAsString("halfy"));
    halfz_ = BambooUtils::evaluate(dp.getParameterAsString("halfz"));
    G4cout << "PtfeBlck with name " << name << " constructed." << G4endl;
}

G4bool PtfeBlock::construct() {
    G4Material *ptfe = G4Material::GetMaterial("G4_TEFLON");
    G4Box *ptfeSolid = new G4Box("PTFESolid", halfx_, halfy_, halfz_);
    partLogicalVolume_ =
        new G4LogicalVolume(ptfeSolid, ptfe, "PTFELog", 0, 0, 0);
    G4VisAttributes *ptfe_vis = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9));
    partLogicalVolume_->SetVisAttributes(ptfe_vis);
    partPhysicalVolume_ =
        new G4PVPlacement(0, G4ThreeVector(), partLogicalVolume_, "PtfeBlock",
                          parentPart_->getContainerLogicalVolume(), false, 0);

    partContainerLogicalVolume_ = nullptr;
    return true;
}
