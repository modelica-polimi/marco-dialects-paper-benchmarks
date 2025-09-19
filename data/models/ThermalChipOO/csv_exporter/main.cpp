#include <iostream>
#include <string>
#include <vector>

struct Config {
    int nx;
    int ny;
    int nz;
    int equations;
    std::string experiment;

    Config(int nx, int ny, int nz, int equations, std::string experiment)
        : nx(nx), ny(ny), nz(nz), equations(equations), experiment(std::move(experiment)) {
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
    configs.emplace_back(4, 4, 4, 1040, experiment);
    configs.emplace_back(6, 6, 4, 2330, experiment);
    configs.emplace_back(8, 8, 4, 4136, experiment);
    configs.emplace_back(12, 12, 4, 9296, experiment);
    configs.emplace_back(15, 15, 5, 17873, experiment);
    configs.emplace_back(18, 18, 6, 30626, experiment);
    configs.emplace_back(24, 24, 8, 71720, experiment);
    configs.emplace_back(33, 33, 11, 184544, experiment);
    configs.emplace_back(39, 39, 13, 303389, experiment);
    configs.emplace_back(54, 54, 18, 800450, experiment);
    configs.emplace_back(66, 66, 22, 1457090, experiment);
    configs.emplace_back(84, 84, 28, 2995280, experiment);
    configs.emplace_back(114, 114, 38, 7466210, experiment);
    configs.emplace_back(144, 144, 48, 15023240, experiment);
    configs.emplace_back(183, 183, 61, 30792869, experiment);
    configs.emplace_back(246, 246, 82, 74707010, experiment);
    configs.emplace_back(312, 312, 104, 152294696, experiment);

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
    return config.experiment + "-" + std::to_string(config.nx) + "-" + std::to_string(config.ny) + "-" + std::to_string(config.nz);
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
    std::cout << "nx,ny,nz,eq,";

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
        std::cout << config.nx << "," << config.ny << "," << config.nz << "," << config.equations << ",";

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
