#ifndef METALSPHERE_H
#define METALSPHERE_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

#include <string>

class MetalSphere : public BambooDetectorPart {

  public:
    MetalSphere(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<MetalSphere> reg;

  private:
    double radius_;
    std::string material_;
};

#endif // METALSPHERE_H
