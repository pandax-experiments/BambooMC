#ifndef SAMPLECYLINDERDETECTOR_H
#define SAMPLECYLINDERDETECTOR_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SampleCylinderDetector : public BambooDetectorPart {
  public:
    SampleCylinderDetector(const G4String &name);
    virtual G4bool construct();
    static DetectorRegister<SampleCylinderDetector> reg;

  private:
    double _radius;
    double _height;
    double _shiftX;
    double _shiftY;
    double _shiftZ;
};

#endif // SAMPLECYLINDERDETECTOR_H
