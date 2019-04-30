#pragma once

#include <string>
#include <vector>

#include <TFile.h>

class TTree;

class G4Event;

class TemporaryParticle {
  public:
    TemporaryParticle() = default;
    ~TemporaryParticle() = default;

    const std::string &getParticleType() const { return type; }
    void setParticleType(const std::string &s) { type = s; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    double getEnergy() const { return energy; }
    void setEnergy(double e) { energy = e; }

    double getPx() const { return px; }
    void setPx(double v) { px = v; }

    double getPy() const { return py; }
    void setPy(double v) { py = v; }

    double getPz() const { return pz; }
    void setPz(double v) { pz = v; }

    double getX() const { return x; }
    void setX(double v) { x = v; }

    double getY() const { return y; }
    void setY(double v) { y = v; }

    double getZ() const { return z; }
    void setZ(double v) { z = v; }

  private:
    std::string type = "";
    int id;
    double energy;
    double px;
    double py;
    double pz;
    double x;
    double y;
    double z;
};

class PandaXDataManager {

    friend class PandaXRunAction;
    friend class PandaXEventAction;
    friend class PandaXAnalysis;

  private:
    PandaXDataManager() = default;
    PandaXDataManager(const PandaXDataManager &) = delete;
    PandaXDataManager &operator=(const PandaXDataManager &) = delete;

  public:
    static PandaXDataManager &getInstance() {
        static PandaXDataManager instance;
        return instance;
    }

    ~PandaXDataManager();

    void book(const std::string &name = "pandaxout.root");

    void save();

    void close();

    void setRecordEnergyDeposition(bool t) { recordEnergyDeposition = t; }
    void setRecordSurfaceFlux(bool t) { recordSurfaceFlux = t; }
    void setRecordPrimaryParticle(bool t) { recordPrimaryParticle = t; }
    void setRecordNullEvent(bool t) { recordNullEvent = t; }

    void fillEvent(const G4Event *aEvent, bool partial = false);

    void addTemporaryParticle(const TemporaryParticle &tp);

  private:
    void resetData();

    //! reset the hits collection of the event
    void resetPartialEvent(const G4Event *aEvent);

    TFile *rootFile = nullptr;
    TTree *mcTree = nullptr;

    int runId;
    int eventId;
    int64_t seed;

    // variables for the energy deposition.
    int nHits;
    double totalEnergy;
    std::vector<int> trackId;
    std::vector<int> parentId;
    std::vector<std::string> type;
    std::vector<std::string> parentType;
    std::vector<std::string> creatorProcess;
    std::vector<std::string> depositionProcess;
    std::vector<std::string> volume;
    std::vector<double> xd;
    std::vector<double> yd;
    std::vector<double> zd;
    std::vector<double> td;
    std::vector<double> energy;

    // variables for flux counts
    int nTracks;
    std::vector<double> trackEnergy;
    std::vector<std::string> trackName;
    std::vector<std::string> trackParent;
    std::vector<double> px;
    std::vector<double> py;
    std::vector<double> pz;
    std::vector<double> track_time;
    std::vector<double> track_x;
    std::vector<double> track_y;
    std::vector<double> track_z;

    // variables for primary particle
    int nPrimaries;
    std::vector<std::string> primaryType;
    std::vector<int> primaryId;
    std::vector<double> primaryEnergy;
    std::vector<double> primaryPx;
    std::vector<double> primaryPy;
    std::vector<double> primaryPz;
    std::vector<double> primaryX;
    std::vector<double> primaryY;
    std::vector<double> primaryZ;

    bool recordEnergyDeposition = true;
    bool recordSurfaceFlux = false;
    bool recordPrimaryParticle = true;
    bool recordNullEvent = false;
    bool partialSaved = false;

    std::vector<TemporaryParticle> particles;
    std::vector<TemporaryParticle> tparticles;
};
