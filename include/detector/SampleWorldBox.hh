#ifndef SAMPLEWORLDBOX_H
#define SAMPLEWORLDBOX_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SampleWorldBox : public BambooDetectorPart {
  public:
    SampleWorldBox(const G4String &name);
    virtual G4bool construct();

    static DetectorRegister<SampleWorldBox> reg;

  private:
    double _halfX;
    double _halfY;
    double _halfZ;
};

#endif // SAMPLEWORLDBOX_H
