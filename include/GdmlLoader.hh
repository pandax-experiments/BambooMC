#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class GdmlLoader : public BambooDetector {

  public:
    GdmlLoader(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<GdmlLoader, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};

