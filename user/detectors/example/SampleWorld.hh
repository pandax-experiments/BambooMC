#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class SampleWorld : public BambooDetector {

  public:
    SampleWorld(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<SampleWorld, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};
