#include "detector/SimpleLabWall.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>

DetectorRegister<SimpleLabWall> SimpleLabWall::reg("SimpleLabWall");

SimpleLabWall::SimpleLabWall(const G4String &name) : BambooDetectorPart(name) {

    DetectorParameters dp =
        BambooGlobalVariables::Instance()->findDetectorPartParameters(
            "SimpleLabWall");

    _length = BambooUtils::evaluate(dp.getParameterAsString("length"));
    _width = BambooUtils::evaluate(dp.getParameterAsString("width"));
    _height = BambooUtils::evaluate(dp.getParameterAsString("height"));

    _rockWallThickness =
        BambooUtils::evaluate(dp.getParameterAsString("rock_wall_thickness"));
    _concreteWallThickness = BambooUtils::evaluate(
        dp.getParameterAsString("concrete_wall_thickness"));
    _concreteFloorThickness = BambooUtils::evaluate(
        dp.getParameterAsString("concrete_floor_thickness"));

    if (_length == 0) {
        _length = 20. * m;
    }
    if (_width == 0) {
        _width = 15. * m;
    }
    if (_height == 0) {
        _height = 14. * m;
    }
    if (_rockWallThickness == 0) {
        _rockWallThickness = 2. * m;
    }
    if (_concreteWallThickness == 0) {
        _concreteWallThickness = 1. * m;
    }
    if (_concreteFloorThickness == 0) {
        _concreteFloorThickness = 0.5 * m;
    }

    _isVisible = false;
    int iV = dp.getParameterAsInt("visible");
    if (iV == 1) {
        _isVisible = true;
    }

    G4cout << "SimpleLabWall found..." << G4endl;
}

G4bool SimpleLabWall::construct() {
    // add code here ...

    double concreteLength = _length + 2. * _concreteWallThickness;
    double concreteWidth = _width + 2. * _concreteWallThickness;
    double concreteHeight = _height + 2 * _concreteFloorThickness;

    double rockLength = concreteLength + 2. * _rockWallThickness;
    double rockWidth = concreteWidth + 2. * _rockWallThickness;
    double rockHeight = concreteHeight + 2. * _rockWallThickness;

    // the rock wall
    G4Material *marble = G4Material::GetMaterial("MARBLE");
    if (marble == 0) {
        G4cout << "Can't find the material MARBLE" << G4endl;
        return false;
    }

    // contruct a rock box
    G4Box *marbleBox = new G4Box("MarbleBox", rockLength / 2., rockWidth / 2.,
                                 rockHeight / 2.);
    _rockWallLogicalVolume =
        new G4LogicalVolume(marbleBox, marble, "MarbleBoxLog", 0, 0, 0);
    partLogicalVolume_ = _rockWallLogicalVolume;

    G4VisAttributes *rockVis = new G4VisAttributes();
    rockVis->SetColour(0.6, 0.6, 0.6, 0.2);
    rockVis->SetVisibility(_isVisible);
    _rockWallLogicalVolume->SetVisAttributes(rockVis);

    _rockWallPhysicalVolume = new G4PVPlacement(
        0, G4ThreeVector(), partLogicalVolume_, "RockWall", 0, false, 0);
    partPhysicalVolume_ = _rockWallPhysicalVolume;

    // the concrete wall
    G4Material *concrete = G4Material::GetMaterial("G4_CONCRETE");
    G4Box *concreteBox = new G4Box("ConcreteBox", concreteLength / 2.,
                                   concreteWidth / 2., concreteHeight / 2.);
    _concreteWallLogicalVolume =
        new G4LogicalVolume(concreteBox, concrete, "ConcreteBoxLog", 0, 0, 0);
    _concreteWallPhysicalVolume =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _concreteWallLogicalVolume,
                          "ConcreteWall", _rockWallLogicalVolume, false, 0);

    G4VisAttributes *concreteVis = new G4VisAttributes();
    concreteVis->SetColour(0.9, 0.9, 0.9, 0.2);
    concreteVis->SetVisibility(_isVisible);
    _concreteWallLogicalVolume->SetVisAttributes(concreteVis);

    partContainerLogicalVolume_ = _concreteWallLogicalVolume;

    G4cout << "Rock Mass: " << _rockWallLogicalVolume->GetMass() / kg << G4endl;
    G4cout << "Concrete Mass: " << _concreteWallLogicalVolume->GetMass() / kg
           << G4endl;
    return true;
}
