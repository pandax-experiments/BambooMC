#include "PandaXEnergyDepositionHit.hh"

G4Allocator<PandaXEnergyDepositionHit> *pandaXEnergyDepositionHitAllocator =
    new G4Allocator<PandaXEnergyDepositionHit>;

PandaXEnergyDepositionHit::PandaXEnergyDepositionHit()
    : trackId_(-1), parentId_(-1), energy_(0), x_(0), y_(0), z_(0),
      type_("unknown"), creatorProcess_("unknown"),
      depositionProcess_("unknown"), parent_("none") {}

const PandaXEnergyDepositionHit &PandaXEnergyDepositionHit::
operator=(const PandaXEnergyDepositionHit &right) {
    trackId_ = right.trackId_;
    parentId_ = right.parentId_;
    energy_ = right.energy_;
    x_ = right.x_;
    y_ = right.y_;
    z_ = right.z_;
    t_ = right.t_;
    type_ = right.type_;
    creatorProcess_ = right.creatorProcess_;
    depositionProcess_ = right.depositionProcess_;
    parent_ = right.parent_;
    return *this;
}

int PandaXEnergyDepositionHit::
operator==(const PandaXEnergyDepositionHit &right) const {
    return ((this == &right) ? 1 : 0);
}

int PandaXEnergyDepositionHit::getTrackId() const { return trackId_; }

int PandaXEnergyDepositionHit::getParentId() const { return parentId_; }

double PandaXEnergyDepositionHit::getEnergy() const { return energy_; }

double PandaXEnergyDepositionHit::getX() const { return x_; }

double PandaXEnergyDepositionHit::getY() const { return y_; }

double PandaXEnergyDepositionHit::getZ() const { return z_; }

double PandaXEnergyDepositionHit::getT() const { return t_; }

const string &PandaXEnergyDepositionHit::getType() const { return type_; }

const string &PandaXEnergyDepositionHit::getCreatorProcess() const {
    return creatorProcess_;
}

const string &PandaXEnergyDepositionHit::getDepositionProcess() const {
    return depositionProcess_;
}

const string &PandaXEnergyDepositionHit::getParent() const { return parent_; }

void PandaXEnergyDepositionHit::setTrackId(int id) { trackId_ = id; }

void PandaXEnergyDepositionHit::setParentId(int id) { parentId_ = id; }

void PandaXEnergyDepositionHit::setEnergy(double energy) { energy_ = energy; }

void PandaXEnergyDepositionHit::setX(double x) { x_ = x; }

void PandaXEnergyDepositionHit::setY(double y) { y_ = y; }

void PandaXEnergyDepositionHit::setZ(double z) { z_ = z; }

void PandaXEnergyDepositionHit::setT(double t) { t_ = t; }

void PandaXEnergyDepositionHit::setType(const string &type) { type_ = type; }

void PandaXEnergyDepositionHit::setCreatorProcess(const string &process) {
    creatorProcess_ = process;
}

void PandaXEnergyDepositionHit::setDepositionProcess(const string &process) {
    depositionProcess_ = process;
}

void PandaXEnergyDepositionHit::setParent(const string &parent) {
    parent_ = parent;
}
