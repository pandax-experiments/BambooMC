#pragma once

#include "BambooMaterial.hh"
#include "BambooFactory.hh"

class SampleMaterial : public BambooMaterial {

  public:
    SampleMaterial() = default;

    virtual void defineMaterials(const BambooParameters &pars);

    static MaterialRegister<SampleMaterial> reg;

  private:
};

