#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class CylinderOp : public BambooDetector {

  public:
    CylinderOp(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<CylinderOp, std::string, BambooParameters> reg;

  private:
    void defineMaterials();
};
