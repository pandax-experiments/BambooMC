#pragma once

#include <string>
#include <vector>

#include <TFile.h>

class TTree;

class G4Event;

class PandaXOpticalDataManager {

    friend class PandaXOpticalRunAction;
    friend class PandaXOpticalEventAction;
    // friend class PandaXAnalysis;

  private:
    PandaXOpticalDataManager() = default;
    PandaXOpticalDataManager(const PandaXOpticalDataManager &) = delete;
    PandaXOpticalDataManager &
    operator=(const PandaXOpticalDataManager &) = delete;

  public:
    static PandaXOpticalDataManager &getInstance() {
        static PandaXOpticalDataManager instance;
        return instance;
    }

    ~PandaXOpticalDataManager();

    void book(const std::string &name = "pandaxout.root");

    void save();

    void close();

    void setRecordEnergyDeposition(bool t) { recordEnergyDeposition = t; }

    void fillEvent(const G4Event *aEvent);

  private:
    void resetData();

    TFile *rootFile = nullptr;
    TTree *mcTree = nullptr;

    int runId;
    int eventId;

    int nHits;
    std::vector<int> pmtNumber;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> energy;
    std::vector<double> velocity;
    std::vector<std::string> creatorProcess;
    std::vector<double> tGlobal;
    std::vector<double> tLocal;
    std::vector<double> tProper;
    std::vector<double> ox;
    std::vector<double> oy;
    std::vector<double> oz;
    std::vector<std::string> parent;
    std::vector<int> photonParentId;

    // variables related to energy deposition
    int nEnergyHits;
    double totalEnergy;
    std::vector<int> trackId;
    std::vector<int> parentId;
    std::vector<std::string> type;
    std::vector<std::string> parentType;
    std::vector<std::string> volume;
    std::vector<std::string> depositionProcess;
    std::vector<double> xd;
    std::vector<double> yd;
    std::vector<double> zd;
    std::vector<double> td;
    std::vector<double> hitEnergy;

    bool recordEnergyDeposition = true;
};
