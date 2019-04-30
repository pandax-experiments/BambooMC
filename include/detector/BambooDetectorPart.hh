#ifndef BAMBOODETECTORPART_H
#define BAMBOODETECTORPART_H

#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <globals.hh>

class G4LogicalVolume;
class G4VPhysicalVolume;

class BambooDetectorPart {
  public:
    BambooDetectorPart(const G4String &partName);

    virtual ~BambooDetectorPart() {}

    virtual G4bool construct() = 0;

    G4String getName() const { return partName_; }

    void setParent(BambooDetectorPart *parent);

    G4LogicalVolume *getLogicalVolume();

    G4LogicalVolume *getContainerLogicalVolume();

    G4VPhysicalVolume *getPhysicalVolume();

  protected:
    G4String partName_;

    G4LogicalVolume *partLogicalVolume_;

    G4LogicalVolume *partContainerLogicalVolume_;

    G4VPhysicalVolume *partPhysicalVolume_;

    BambooDetectorPart *parentPart_;
};
#endif
