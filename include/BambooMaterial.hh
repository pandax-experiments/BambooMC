#pragma once

#include "BambooControl.hh"

class BambooMaterial {
  public:
    BambooMaterial() = default;

    virtual ~BambooMaterial() = default;

    virtual void defineMaterials(const BambooParameters &pars) = 0;

  protected:

};
