#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooFactory.hh"
#include "BambooUtils.hh"

#include "external/json.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <unistd.h>

#include <QFile>
#include <QXmlStreamReader>

#include <G4PhysListFactory.hh>
#include <G4VUserPhysicsList.hh>

const std::string
BambooParameters::getParameter(const std::string &name) const {
    if (parameters.find(name) != parameters.end())
        return parameters.at(name);
    return "";
}

double BambooParameters::evaluateParameter(const std::string &name) const {
    if (parameters.find(name) != parameters.end())
        return BambooUtils::evaluate(parameters.at(name));
    return 0;
}

void BambooControl::setup(int argc, char *argv[]) {
    extern char *optarg;
    std::stringstream ss;
    while (true) {
        const int option = getopt(argc, argv, "m:n:o:c:x:ig:");
        if (option == -1)
            break;
        switch (option) {
        case 'm':
            mac_file_name = optarg;
            break;
        case 'n':
            ss.clear();
            ss.str(optarg);
            ss >> num_events;
            break;
        case 'o':
            data_file_name = optarg;
            analysisParameters.addParameter("DataFileName", data_file_name);
            break;
        case 'c':
        case 'x':
            config_file_name = optarg;
            if (!loadConfig(config_file_name))
                exit(1);
            break;
        case 'i':
            interactive = true;
            break;
        case 'g':
            gdml_file_name = optarg;
        }
    }

    if (config_file_name.empty()) {
        std::cerr << "A config file must be provided! Try '-c config_file'."
                  << std::endl;
        exit(1);
    }

    if (optind <= argc - 1) {
        std::cerr << optind << " vs " << argc - 1 << std::endl;
        std::cerr << "Unrecognized arguments!" << std::endl;
        exit(1);
    }
}

bool BambooControl::loadConfig(const std::string &config_name) {
    auto n = config_name.find_last_of('.');
    if (n == std::string::npos)
        return false;
    auto extension_name = config_name.substr(n + 1);
    if (extension_name == "xml") {
        std::cout << "load xml config" << std::endl;
        return loadXmlConfig(config_name);
    } else if (extension_name == "json") {
        std::cout << "load json config" << std::endl;
        return loadJsonConfig(config_name);
    }
#ifdef WITH_YAML_CPP
    else if (extension_name == "yml" || extension_name == "yaml") {
        std::cout << "load yaml config" << std::endl;
        return loadYamlConfig(config_name);
    }
#endif
    else {
        std::cout << "not supported!" << std::endl;
        return false;
    }
    return false;
}

auto extract_attributes(QXmlStreamReader &reader, const std::string &name) {
    return reader.attributes().value(name.c_str()).toString().toStdString();
}

auto read_parameter(QXmlStreamReader &reader) {
    auto parameter = std::make_pair(extract_attributes(reader, "name"),
                                    extract_attributes(reader, "value"));
    return parameter;
}

void read_entry(QXmlStreamReader &reader, BambooParameters &pars) {
    while (reader.readNextStartElement()) {
        if (reader.name() == "parameter") {
            auto par = read_parameter(reader);
            pars.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        }
    }
}

DetectorInfoTuple BambooControl::read_detector_xml(QXmlStreamReader &reader) {
    auto name = extract_attributes(reader, "name");
    auto type = extract_attributes(reader, "type");
    auto parent = extract_attributes(reader, "parent");
    if (name == "") {
        name = type;
    } else if (type == "") {
        type = name;
    }
    detectorParameterMaps.emplace(name, BambooParameters{});
    while (reader.readNextStartElement()) {
        auto par = read_parameter(reader);
        detectorParameterMaps[name].addParameter(par.first, par.second);
        reader.skipCurrentElement();
    }
    return std::make_tuple(name, type, parent);
}

void BambooControl::read_geometry_xml(QXmlStreamReader &reader) {
    std::set<DetectorInfoTuple> det_set;
    while (reader.readNextStartElement()) {
        if (reader.name() == "parameter") {
            auto par = read_parameter(reader);
            geometryParameters.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        } else if (reader.name() == "material") {
            materialName = extract_attributes(reader, "name");
            read_entry(reader, materialParameters);
        } else if (reader.name() == "detector") {
            det_set.emplace(read_detector_xml(reader));
        }
    }
    if (!sortDetectors(det_set)) {
        std::cerr << "invalid detector.." << std::endl;
        exit(1);
    }
}

bool BambooControl::loadXmlConfig(const std::string &config_name) {
    QFile file(QString(config_name.c_str()));
    if (!file.exists()) {
        std::cerr << "File " << config_name << " does not exist!" << std::endl;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "can't open " << config_name << std::endl;
        return false;
    }
    QXmlStreamReader reader(&file);
    if (reader.readNextStartElement()) {
        if (reader.name() != "BambooMC") {
            std::cerr << "No entry for BambooMC!" << std::endl;
            return false;
        }
    } else {
        if (reader.hasError()) {
            std::cerr << "Error Code = " << reader.error() << std::endl;
        }
        std::cerr << "Can not read from file " << config_name << std::endl;
        return false;
    }
    while (reader.readNextStartElement()) {
        if (reader.name() == "run") {
            run_number = std::stoi(extract_attributes(reader, "number"));
            reader.skipCurrentElement();
        } else if (reader.name() == "geometry") {
            read_geometry_xml(reader);
        } else if (reader.name() == "physics") {
            physicsName = extract_attributes(reader, "name");
            read_entry(reader, physicsParameters);
        } else if (reader.name() == "generator") {
            generatorName = extract_attributes(reader, "name");
            read_entry(reader, generatorParameters);
        } else if (reader.name() == "analysis") {
            analysisName = extract_attributes(reader, "name");
            read_entry(reader, analysisParameters);
        } else {
            reader.skipCurrentElement();
        }
    }
    return true;
}

void read_pars_json(const json &j, BambooParameters &pars) {
    for (const auto &el : j.items()) {
        auto key = el.key();
        auto value = el.value();
        if (value.is_string())
            pars.addParameter(key, value);
        else
            pars.addParameter(key, value.dump());
    }
}

DetectorInfoTuple BambooControl::read_detector_json(const json &j) {
    std::string name, type, parent;
    if (j.contains("name"))
        name = j["name"];
    if (j.contains("type"))
        type = j["type"];
    if (j.contains("parent"))
        parent = j["parent"];
    if (name == "")
        name = type;
    else if (type == "")
        type = name;
    detectorParameterMaps.emplace(name, BambooParameters{});
    if (j.contains("parameters")) {
        for (auto &p : j["parameters"].items()) {
            auto key = p.key();
            auto value = p.value();
            if (value.is_string())
                detectorParameterMaps[name].addParameter(key, value);
            else
                detectorParameterMaps[name].addParameter(key, value.dump());
        }
    }
    return std::make_tuple(name, type, parent);
}

void BambooControl::read_geometry_json(const json &j) {
    std::set<DetectorInfoTuple> det_set;
    for (const auto &el : j.items()) {
        if (el.key() == "parameters") {
            read_pars_json(el.value(), geometryParameters);
        } else if (el.key() == "material") {
            for (const auto &p : el.value().items()) {
                if (p.key() == "name")
                    materialName = p.value();
                else if (p.key() == "parameters")
                    read_pars_json(p.value(), materialParameters);
            }
        } else if (el.key() == "detectors") {
            for (const auto &p : el.value()) {
                det_set.emplace(read_detector_json(p));
            }
        }
    }
    if (!sortDetectors(det_set)) {
        std::cerr << "invalid detector.." << std::endl;
        exit(1);
    }
}

bool BambooControl::loadJsonConfig(const std::string &config_name) {
    std::ifstream iconfig(config_name);
    if (!iconfig.is_open()) {
        std::cerr << "File " << config_name << " does not exist!" << std::endl;
        return false;
    }
    json j;
    try {
        iconfig >> j;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    for (auto &el : j.items()) {
        if (el.key() == "run") {
            run_number = el.value();
        } else if (el.key() == "geometry") {
            read_geometry_json(el.value());
        } else if (el.key() == "physics") {
            physicsName = el.value()["name"];
            if (el.value().contains("parameters")) {
                read_pars_json(el.value()["parameters"], physicsParameters);
            }
        } else if (el.key() == "generator") {
            generatorName = el.value()["name"];
            if (el.value().contains("parameters")) {
                read_pars_json(el.value()["parameters"], generatorParameters);
            }
        } else if (el.key() == "analysis") {
            analysisName = el.value()["name"];
            if (el.value().contains("parameters")) {
                read_pars_json(el.value()["parameters"], analysisParameters);
            }
        } else {
        }
    }
    return true;
}

#ifdef WITH_YAML_CPP
DetectorInfoTuple BambooControl::read_detector_yaml(const YAML::Node &node) {
    std::string name, type, parent;
    if (node["name"])
        name = node["name"].as<std::string>();
    if (node["type"])
        type = node["type"].as<std::string>();
    if (node["parent"])
        parent = node["parent"].as<std::string>();
    if (name == "")
        name = type;
    else if (type == "")
        type = name;
    detectorParameterMaps.emplace(name, BambooParameters{});
    if (node["parameters"]) {
        for (const auto &p : node["parameters"]) {
            auto key = p.first.as<std::string>();
            auto value = p.second.as<std::string>();
            detectorParameterMaps[name].addParameter(key, value);
        }
    }
    return std::make_tuple(name, type, parent);
}

void read_pars_yaml(const YAML::Node &node, BambooParameters &pars) {
    for (const auto &p : node) {
        auto key = p.first.as<std::string>();
        auto value = p.second.as<std::string>();
        pars.addParameter(key, value);
    }
}

void BambooControl::read_geometry_yaml(const YAML::Node &geo) {
    std::set<DetectorInfoTuple> det_set;
    for (auto it = geo.begin(); it != geo.end(); ++it) {
        auto key = it->first.as<std::string>();
        if (key == "material") {
            if (it->second["name"]) {
                materialName = it->second["name"].as<std::string>();
            }
            if (it->second["parameters"]) {
                read_pars_yaml(it->second["parameters"], materialParameters);
            }
        } else if (key == "detectors") {
            for (const auto &d : it->second) {
                det_set.emplace(read_detector_yaml(d));
            }
        } else if (key == "parameters") {
            read_pars_yaml(it->second, geometryParameters);
        }
    }
    if (!sortDetectors(det_set)) {
        std::cerr << "invalid detector.." << std::endl;
        exit(1);
    }
}

bool BambooControl::loadYamlConfig(const std::string &config_name) {
    try {
        YAML::Node config = YAML::LoadFile(config_name);
        if (config["run"]) {
            run_number = config["run"].as<int>();
        }
        if (config["geometry"]) {
            auto geo = config["geometry"];
            read_geometry_yaml(geo);
        }
        if (config["physics"]) {
            physicsName = config["physics"]["name"].as<std::string>();
            if (config["physics"]["parameters"]) {
                read_pars_yaml(config["physics"]["parameters"],
                               physicsParameters);
            }
        }
        if (config["generator"]) {
            generatorName = config["generator"]["name"].as<std::string>();
            if (config["generator"]["parameters"]) {
                read_pars_yaml(config["generator"]["parameters"],
                               generatorParameters);
            }
        }
        if (config["analysis"]) {
            analysisName = config["analysis"]["name"].as<std::string>();
            if (config["analysis"]["parameters"]) {
                read_pars_yaml(config["analysis"]["parameters"],
                               analysisParameters);
            }
        }
        return true;
    } catch (YAML::Exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
#endif

bool BambooControl::sortDetectors(std::set<DetectorInfoTuple> &ds) {
    std::vector<std::string> names{{""}};
    using factory = DetectorFactory<std::string, BambooParameters>;
    auto detectorExist{true};
    while (!ds.empty()) {
        auto it = std::find_if(ds.cbegin(), ds.cend(), [&names](const auto &t) {
            return std::get<2>(t) == names.back();
        });
        if (it == ds.cend()) {
            if (names.back() == "")
                break;
            names.pop_back();
            continue;
        }
        names.emplace_back(std::get<0>(*it));
        auto type = std::get<1>(*it);
        if (detectorExist && !factory::exists(type)) {
            std::cerr << "detector type " << type << " does not exist! "
                      << std::endl;
            detectorExist = false;
        }
        detectorInfo.emplace_back(std::move(*it));
        ds.erase(it);
    }
    if (!ds.empty())
        return false;
    return detectorExist;
}

void BambooControl::print() const {
    std::cout << "run: " << run_number << std::endl;

    if (materialName != "") {
        std::cout << "material: " << materialName << std::endl;
    }

    for (const auto &par : geometryParameters.getParameters()) {
        std::cout << "global geometry parameter: " << par.first << " = "
                  << par.second << std::endl;
    }

    for (const auto &di : detectorInfo) {
        const auto &name = std::get<0>(di);
        const auto &type = std::get<1>(di);
        const auto &parent = std::get<2>(di);
        std::cout << "detector: " << name << " (" << type << ")";
        if (parent != "") {
            std::cout << " (in " << parent << ")";
        }
        std::cout << std::endl;
        if (detectorParameterMaps.find(name) != detectorParameterMaps.end()) {
            const auto &pars = detectorParameterMaps.at(name);
            for (const auto &par : pars.getParameters()) {
                std::cout << " -- parameter: " << par.first << " = "
                          << par.second << std::endl;
            }
        }
    }

    std::cout << "physics: " << physicsName << std::endl;
    for (const auto &par : physicsParameters.getParameters()) {
        std::cout << "physics parameter: " << par.first << " = " << par.second
                  << std::endl;
    }
    std::cout << "generator: " << generatorName << std::endl;
    for (const auto &par : generatorParameters.getParameters()) {
        std::cout << "generator parameter: " << par.first << " = " << par.second
                  << std::endl;
    }
    std::cout << "analysis: " << analysisName << std::endl;
    for (const auto &par : analysisParameters.getParameters()) {
        std::cout << "analysis parameter: " << par.first << " = " << par.second
                  << std::endl;
    }
}

G4VUserDetectorConstruction *BambooControl::createDetector() {
    return new BambooDetectorConstruction{materialName, materialParameters,
                                          geometryParameters, detectorInfo,
                                          detectorParameterMaps};
}

G4VUserPhysicsList *BambooControl::createPhysics() {
    if (PhysicsFactory::exists(physicsName)) {
        auto physicsList =
            PhysicsFactory::create(physicsName, physicsParameters);
        if (physicsList.get() == nullptr)
            return nullptr;
        return static_cast<G4VUserPhysicsList *>(physicsList.release());
    }
    // find the physics name in G4PhysListFactory
    G4PhysListFactory factory;

    auto p = factory.GetReferencePhysList(physicsName);
    if (p == 0)
        return nullptr;
    return p;
}

G4VUserPrimaryGeneratorAction *BambooControl::createGenerator() {
    auto generator =
        GeneratorFactory::create(generatorName, generatorParameters);
    if (generator.get() == nullptr) {
        return nullptr;
    }
    return static_cast<G4VUserPrimaryGeneratorAction *>(generator.release());
}

std::unique_ptr<BambooAnalysis> BambooControl::createAnalysis() {
    return AnalysisFactory::create(analysisName, analysisParameters);
}
