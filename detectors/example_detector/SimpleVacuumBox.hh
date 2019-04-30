#ifndef SIMPLEVACUUMBOX_H
#define SIMPLEVACUUMBOX_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class SimpleVacuumBox : public BambooDetectorPart {

  public:
    SimpleVacuumBox(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<SimpleVacuumBox> reg;

  private:
    double halfx_ = 2.0 * m;
    double halfy_ = 2.0 * m;
    double halfz_ = 2.0 * m;
};

#endif // SIMPLEVACUUMBOX_H
