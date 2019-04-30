#ifndef PTFEBLOCK_H
#define PTFEBLOCK_H

#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooDetectorPart.hh"

class PtfeBlock : public BambooDetectorPart {

  public:
    PtfeBlock(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<PtfeBlock> reg;

  private:
    double halfx_;
    double halfy_;
    double halfz_;
};

#endif // PTFEBLOCK_H
