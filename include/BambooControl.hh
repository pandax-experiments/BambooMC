#pragma once

#include <type_traits>

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <G4VPhysicalVolume.hh>

#include "detector/BambooDetector.hh"

class QXmlStreamReader;

class G4VUserDetectorConstruction;

class BambooParameters {
  public:
    BambooParameters() = default;

    template <typename Number>
    typename std::enable_if<std::is_arithmetic<Number>::value, Number>::type
    getParameter(const std::string &name) const {
        Number v;
        if (parameters.find(name) == parameters.end())
            return 0;
        std::istringstream ss(parameters.at(name));
        ss >> v;
        return v;
    }

    const std::string getParameter(const std::string &name) const;

    double evaluateParameter(const std::string &name) const;

    bool addParameter(const std::string &name, const std::string &value);

    using par_maps = std::map<std::string, std::string>;
    const par_maps &getParameters() const { return parameters; }

  private:
    std::map<std::string, std::string> parameters;
};

using DetectorInfoTuple = std::tuple<std::string, std::string, std::string>;
using DetectorInfoVec = std::vector<DetectorInfoTuple>;

class BambooControl {
  public:
    BambooControl() = default;
    void setup(int argc, char *argv[]);

    bool isInterative() const { return interactive; }

    int nEvents() const { return num_events; }

    const std::string &getMacFileName() const { return mac_file_name; }
    const std::string &getDataFileName() const { return data_file_name; }
    const std::string &getConfigFileName() const { return config_file_name; }

    int getRunNumber() const { return run_number; }

    void print() const;

    G4VUserDetectorConstruction *createDetector();
    //    G4VPhysicalVolume *constructDetector();

  private:
    bool loadConfig(const std::string &config_name);
    bool loadXmlConfig(const std::string &config_name);
    void read_geometry_xml(QXmlStreamReader &reader);
    DetectorInfoTuple read_detector_xml(QXmlStreamReader &reader);

    bool sortDetectors(std::set<DetectorInfoTuple> &set);

    bool interactive = false;
    int num_events = 1;
    int run_number = 1;
    std::string mac_file_name;
    std::string data_file_name;
    std::string config_file_name;

    std::string physicsName;
    std::string generatorName;
    std::string analysisName;

    BambooParameters geometryParameters;
    BambooParameters analysisParameters;
    BambooParameters physicsParameters;
    BambooParameters generatorParameters;

    DetectorInfoVec detectorInfo;
    std::map<std::string, BambooParameters> detectorParameterMaps;
};
