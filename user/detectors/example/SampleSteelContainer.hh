#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class SampleSteelContainer : public BambooDetector {

  public:
    SampleSteelContainer(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<SampleSteelContainer, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};

