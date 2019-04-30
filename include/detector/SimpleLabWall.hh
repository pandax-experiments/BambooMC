#ifndef SIMPLELABWALL_H
#define SIMPLELABWALL_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SimpleLabWall : public BambooDetectorPart {

  public:
    SimpleLabWall(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<SimpleLabWall> reg;

  private:
    double _length; //!< length of the inner space of the lab
    double _width;  //!< width of the inner space of the lab
    double _height; //!< height of the inner space of the lab

    double _rockWallThickness; //!< thickness of the rock wall

    double _concreteWallThickness;  //!< thickness of the concrete wall
    double _concreteFloorThickness; //!< thickness of the concrete floor

    G4LogicalVolume *_rockWallLogicalVolume; //!< the logical volume, rock wall
    G4LogicalVolume
        *_concreteWallLogicalVolume; //!< the logical volume, concrete wall

    G4VPhysicalVolume
        *_rockWallPhysicalVolume; //!< the physical volume, rock wall
    G4VPhysicalVolume
        *_concreteWallPhysicalVolume; //!< the physical volume, concrete wall

    bool _isVisible;
};

#endif // SIMPLELABWALL_H
