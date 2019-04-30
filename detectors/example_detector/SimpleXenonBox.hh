#ifndef SIMPLEXENONBOX_H
#define SIMPLEXENONBOX_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SimpleXenonBox : public BambooDetectorPart {

  public:
    SimpleXenonBox(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<SimpleXenonBox> reg;

  private:
    double halfx_ = 2.0 * m;
    double halfy_ = 2.0 * m;
    double halfz_ = 2.0 * m;
};

#endif // SIMPLEXENONBOX_H
