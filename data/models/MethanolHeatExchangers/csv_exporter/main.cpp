#include <iostream>
#include <string>
#include <vector>

struct Config {
    int Nu;
    int Nh;
    int Nv;
    int equations;
    std::string experiment;

    Config(int Nu, int Nh, int Nv, std::string experiment)
        : Nu(Nu), Nh(Nh), Nv(Nv), equations(2 + Nh + Nh + Nh * (Nu - 1) + Nu * Nh * Nv + Nu + 3 * Nu * Nh * Nv + 3 * Nu + Nu * Nh * Nv * 2 + Nu * Nh * (Nv + 1)), experiment(std::move(experiment)) {
    }
};

struct TimeMeasure {
    double real{-1};
    double user{-1};
    double sys{-1};
};

struct MARCOStatistics {
    double instantiationTime{-1};
    double compilationTime{-1};
    long long bmodelicaSize{-1};
    long long llvmirSize{-1};
    long long binarySize{-1};
    double simulationTime{-1};

    void reset() {
        instantiationTime = -1;
        compilationTime = -1;
        bmodelicaSize = -1;
        llvmirSize = -1;
        binarySize = -1;
        simulationTime = -1;
    }
};

struct OMCStatistics {
    double compilationTime{-1};
    double cSize{-1};
    double binarySize{-1};
    double simulationTime{-1};

    void reset() {
        compilationTime = -1;
        cSize = -1;
        binarySize = -1;
        simulationTime = -1;
    }
};

void run(const std::string& statsDir, const std::vector<Config>& configs);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " stats_dir experiment" << std::endl;
        return 0;
    }

    std::string statsDir(argv[1]);
    std::string experiment(argv[2]);

    std::vector<Config> configs;
    configs.emplace_back(3, 4, 6, experiment);
    configs.emplace_back(4, 5, 5, experiment);
    configs.emplace_back(4, 6, 6, experiment);
    configs.emplace_back(4, 8, 8, experiment);
    configs.emplace_back(6, 10, 10, experiment);
    configs.emplace_back(6, 12, 12, experiment);
    configs.emplace_back(6, 16, 16, experiment);
    configs.emplace_back(6, 24, 24, experiment);
    configs.emplace_back(8, 24, 24, experiment);
    configs.emplace_back(8, 32, 32, experiment);
    configs.emplace_back(8, 48, 48, experiment);
    configs.emplace_back(12, 64, 64, experiment);
    configs.emplace_back(16, 86, 86, experiment);
    configs.emplace_back(16, 128, 128, experiment);
    configs.emplace_back(16, 192, 192, experiment);
    configs.emplace_back(24, 192, 192, experiment);

    run(statsDir, configs);

    return 0;
}

TimeMeasure parseTimeMeasure(FILE* f) {
    TimeMeasure result;

    fscanf(f, " real %lf", &result.real);
    fscanf(f, " user %lf", &result.user);
    fscanf(f, " sys %lf", &result.sys);

    return result;
}

long long parseSize(FILE* f) {
    long long size = -1;
    fscanf(f, "%lld", &size);
    return size;
}

std::string getConfigString(const Config& config) {
    return config.experiment + "-" + std::to_string(config.Nu) + "-" + std::to_string(config.Nh) + "-" + std::to_string(config.Nv);
}

void parseMARCOStatistics(const std::string& statsDir, const Config& config, MARCOStatistics& stats) {
    std::string configString = getConfigString(config);
    std::string instantiationTimePath = statsDir + "/marco/" + "omc-instantiation-time-" + configString + ".txt";
    std::string compilationTimePath = statsDir + "/marco/" + "marco-compilation-time-" + configString + ".txt";
    std::string bmodelicaSizePath = statsDir + "/marco/" + "marco-bmodelica-size-" + configString + ".txt";
    std::string llvmirSizePath = statsDir + "/marco/" + "marco-llvmir-size-" + configString + ".txt";
    std::string binarySizePath = statsDir + "/marco/" + "marco-binary-size-" + configString + ".txt";
    std::string simulationTimePath = statsDir + "/marco/" + "marco-simulation-time-" + configString + ".txt";

    FILE* fInstantiationTime = fopen(instantiationTimePath.c_str(), "r");
    FILE* fCompilationTime = fopen(compilationTimePath.c_str(), "r");
    FILE* fBmodelicaSize = fopen(bmodelicaSizePath.c_str(), "r");
    FILE* fLLVMIRSize = fopen(llvmirSizePath.c_str(), "r");
    FILE* fBinarySize = fopen(binarySizePath.c_str(), "r");
    FILE* fSimulationTime = fopen(simulationTimePath.c_str(), "r");
    
    stats.reset();

    if (fInstantiationTime) {
        stats.instantiationTime = parseTimeMeasure(fInstantiationTime).real;
    }


    if (fCompilationTime) {
        stats.compilationTime = parseTimeMeasure(fCompilationTime).real;
    }


    if (fBmodelicaSize) {
        stats.bmodelicaSize = parseSize(fBmodelicaSize);
    }


    if (fLLVMIRSize) {
        stats.llvmirSize = parseSize(fLLVMIRSize);
    }


    if (fBinarySize) {
        stats.binarySize = parseSize(fBinarySize);
    }


    if (fSimulationTime) {
        stats.simulationTime = parseTimeMeasure(fSimulationTime).real;
    }
}

void parseOMCStatistics(const std::string& statsDir, const Config& config, OMCStatistics& stats) {
    std::string configString = getConfigString(config);
    std::string compilationTimePath = statsDir + "/omc/" + "omc-compilation-time-" + configString + ".txt";
    std::string cSizePath = statsDir + "/omc/" + "omc-c-size-" + configString + ".txt";
    std::string binarySizePath = statsDir + "/omc/" + "omc-binary-size-" + configString + ".txt";
    std::string simulationTimePath = statsDir + "/omc/" + "omc-simulation-time-" + configString + ".txt";

    FILE* fCompilationTime = fopen(compilationTimePath.c_str(), "r");
    FILE* fCSize = fopen(cSizePath.c_str(), "r");
    FILE* fBinarySize = fopen(binarySizePath.c_str(), "r");
    FILE* fSimulationTime = fopen(simulationTimePath.c_str(), "r");

    stats.reset();

    if (fCompilationTime) {
        stats.compilationTime = parseTimeMeasure(fCompilationTime).real;
    }


    if (fCSize) {
        stats.cSize = parseSize(fCSize);
    }


    if (fBinarySize) {
        stats.binarySize = parseSize(fBinarySize);
    }


    if (fSimulationTime) {
        stats.simulationTime = parseTimeMeasure(fSimulationTime).real;
    }
}

template<typename T>
void printValue(std::ostream& os, T value) {
    if (value != -1) {
        os << value;
    }

    os << ",";
}

void run(const std::string& statsDir, const std::vector<Config>& configs) {
    std::cout << "Nu,Nh,Nv,eq,";

    std::cout << "[MARCO] Instantiation time,";
    std::cout << "[MARCO] Compilation time,";
    std::cout << "[MARCO] bmodelica size,";
    std::cout << "[MARCO] LLVM-IR size,";
    std::cout << "[MARCO] Binary size,";
    std::cout << "[MARCO] Simulation time,";
    std::cout << "[OMC] Compilation time,";
    std::cout << "[OMC] C size,";
    std::cout << "[OMC] Binary size,";
    std::cout << "[OMC] Simulation time,";

    std::cout << std::endl;

    for (const Config& config : configs) {
        std::cout << config.Nu << "," << config.Nh << "," << config.Nv << "," << config.equations << ",";

        MARCOStatistics marco;
        OMCStatistics omc;

        parseMARCOStatistics(statsDir, config, marco);
        parseOMCStatistics(statsDir, config, omc);

        printValue(std::cout, marco.instantiationTime);
        printValue(std::cout, marco.compilationTime);
        printValue(std::cout, marco.bmodelicaSize);
        printValue(std::cout, marco.llvmirSize);
        printValue(std::cout, marco.binarySize);
        printValue(std::cout, marco.simulationTime);

        printValue(std::cout, omc.compilationTime);
        printValue(std::cout, omc.cSize);
        printValue(std::cout, omc.binarySize);
        printValue(std::cout, omc.simulationTime);

        std::cout << std::endl;
    }
}
