#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class SampleWater : public BambooDetector {

  public:
    SampleWater(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<SampleWater, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};

