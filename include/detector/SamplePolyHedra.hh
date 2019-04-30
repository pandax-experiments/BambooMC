#ifndef SAMPLEPOLYHEDRA_H
#define SAMPLEPOLYHEDRA_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SamplePolyHedra : public BambooDetectorPart {

  public:
    SamplePolyHedra(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<SamplePolyHedra> reg;

  private:
    int _nSide;
    double _innerRadius;
    double _outerRadius;
};

#endif // SAMPLEPOLYHEDRA_H
