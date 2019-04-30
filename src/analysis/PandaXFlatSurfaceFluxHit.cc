#include "analysis/PandaXFlatSurfaceFluxHit.hh"

G4Allocator<PandaXFlatSurfaceFluxHit> *pandaXFlatSurfaceFluxHitAllocator =
    new G4Allocator<PandaXFlatSurfaceFluxHit>;

PandaXFlatSurfaceFluxHit::PandaXFlatSurfaceFluxHit()
    : _energy(0), _surface(0) {}

PandaXFlatSurfaceFluxHit::~PandaXFlatSurfaceFluxHit() {}

const PandaXFlatSurfaceFluxHit &PandaXFlatSurfaceFluxHit::
operator=(const PandaXFlatSurfaceFluxHit &right) {
    _energy = right._energy;
    _momentum = right._momentum;
    _surface = right._surface;
    return *this;
}

int PandaXFlatSurfaceFluxHit::
operator==(const PandaXFlatSurfaceFluxHit &right) const {
    return ((this == &right) ? 1 : 0);
}

double PandaXFlatSurfaceFluxHit::getEnergy() const { return _energy; }

std::string PandaXFlatSurfaceFluxHit::getTrackName() const {
    return _trackName;
}

std::string PandaXFlatSurfaceFluxHit::getParentName() const {
    return _parentName;
}

G4ThreeVector PandaXFlatSurfaceFluxHit::getMomentum() const {
    return _momentum;
}

int PandaXFlatSurfaceFluxHit::getSurface() const { return _surface; }

void PandaXFlatSurfaceFluxHit::setEnergy(double energy) { _energy = energy; }

void PandaXFlatSurfaceFluxHit::setTrackName(const std::string &name) {
    _trackName = name;
}

void PandaXFlatSurfaceFluxHit::setParentName(const std::string &name) {
    _parentName = name;
}

void PandaXFlatSurfaceFluxHit::setMomentum(const G4ThreeVector &momentum) {
    _momentum = momentum;
}

void PandaXFlatSurfaceFluxHit::setSurface(int s) { _surface = s; }
