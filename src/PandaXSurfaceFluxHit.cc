#include "PandaXSurfaceFluxHit.hh"

G4Allocator<PandaXSurfaceFluxHit> *pandaXSurfaceFluxHitAllocator =
    new G4Allocator<PandaXSurfaceFluxHit>;

const PandaXSurfaceFluxHit &PandaXSurfaceFluxHit::
operator=(const PandaXSurfaceFluxHit &right) {
    energy_ = right.energy_;
    momentum_ = right.momentum_;
    return *this;
}

int PandaXSurfaceFluxHit::operator==(const PandaXSurfaceFluxHit &right) const {
    return ((this == &right) ? 1 : 0);
}

double PandaXSurfaceFluxHit::getEnergy() const { return energy_; }

std::string PandaXSurfaceFluxHit::getTrackName() const { return trackName_; }

std::string PandaXSurfaceFluxHit::getParentName() const { return parentName_; }

G4ThreeVector PandaXSurfaceFluxHit::getMomentum() const { return momentum_; }

void PandaXSurfaceFluxHit::setEnergy(double energy) { energy_ = energy; }

void PandaXSurfaceFluxHit::setTrackName(const std::string &name) {
    trackName_ = name;
}

void PandaXSurfaceFluxHit::setParentName(const std::string &name) {
    parentName_ = name;
}

void PandaXSurfaceFluxHit::setMomentum(const G4ThreeVector &momentum) {
    momentum_ = momentum;
}
