#ifndef SAMPLEBOXDETECTOR_H
#define SAMPLEBOXDETECTOR_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SampleBoxDetector : public BambooDetectorPart {
  public:
    SampleBoxDetector(const G4String &name);
    virtual G4bool construct();

    static DetectorRegister<SampleBoxDetector> reg;

  private:
    double _halfX;
    double _halfY;
    double _halfZ;
};

#endif // SAMPLEBOXDETECTOR_H
