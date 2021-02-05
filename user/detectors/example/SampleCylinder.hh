#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class SampleCylinder : public BambooDetector {

  public:
    SampleCylinder(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<SampleCylinder, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};

