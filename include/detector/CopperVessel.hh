#ifndef COPPERVESSEL_H
#define COPPERVESSEL_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class CopperVessel : public BambooDetectorPart {

  public:
    CopperVessel(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<CopperVessel> reg;

  private:
    double _outerRadius;
    double _wallThickness;
    double _topThickness;
    double _height;
    double _shiftX;
    double _shiftY;
    double _shiftZ;
};

#endif // COPPERVESSEL_H
