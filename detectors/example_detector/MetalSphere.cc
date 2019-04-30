#include "MetalSphere.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "detector/BambooDetectorFactory.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4Sphere.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<MetalSphere> MetalSphere::reg("MetalSphere");

MetalSphere::MetalSphere(const G4String &name) : BambooDetectorPart(name) {
    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "MetalSphere");

    radius_ = BambooUtils::evaluate(dp.getParameterAsString("radius"));
    material_ = dp.getParameterAsString("material");
    if (radius_ <= 0)
        radius_ = 1.0 * cm;
    if (material_ != "copper" && material_ != "stainless_steel")
        material_ = "copper";
    G4cout << "MetalSphere with name " << name << " constructed." << G4endl;
}

G4bool MetalSphere::construct() {
    G4Material *metal;
    if (material_ == "copper") {
        metal = G4Material::GetMaterial("G4_Cu");
    } else if (material_ == "stainless_steel") {
        metal = G4Material::GetMaterial("G4_STAINLESS-STEEL");
    }
    G4Sphere *metalSolid =
        new G4Sphere("MetalSolid", 0, radius_, 0, 2.0 * pi, 0, pi);
    partLogicalVolume_ =
        new G4LogicalVolume(metalSolid, metal, "MetalLog", 0, 0, 0);
    G4VisAttributes *metal_vis = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
    partLogicalVolume_->SetVisAttributes(metal_vis);
    partPhysicalVolume_ =
        new G4PVPlacement(0, G4ThreeVector(), partLogicalVolume_, "MetalSphere",
                          parentPart_->getContainerLogicalVolume(), false, 0);

    partContainerLogicalVolume_ = nullptr;
    return true;
}
