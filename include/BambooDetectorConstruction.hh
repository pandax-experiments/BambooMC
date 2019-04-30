#ifndef BAMBOODETECTORCONSTRUCTION_H
#define BAMBOODETECTORCONSTRUCTION_H

#include <G4Material.hh>
#include <G4RotationMatrix.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4VisAttributes.hh>

class G4VPhysicalVolume;

class BambooDetectorConstruction : public G4VUserDetectorConstruction {
  public:
    BambooDetectorConstruction();
    virtual ~BambooDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

  private:
    void DefineMaterials();
};
#endif
