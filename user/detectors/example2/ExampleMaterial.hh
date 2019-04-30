#pragma once

#include "BambooMaterial.hh"
#include "BambooFactory.hh"

class ExampleMaterial : public BambooMaterial {

  public:
    ExampleMaterial() = default;

    virtual void defineMaterials(const BambooParameters &pars);

    static MaterialRegister<ExampleMaterial> reg;

  private:
};

