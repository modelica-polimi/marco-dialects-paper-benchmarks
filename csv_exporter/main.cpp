#include <iostream>
#include <string>
#include <vector>

struct Config {
    int nx;
    int ny;
    int nz;
    int equations;
    std::string solver;

    Config(int nx, int ny, int nz, int equations, std::string solver) : nx(nx), ny(ny), nz(nz), equations(equations), solver(std::move(solver)) {
    }
};

struct TimeMeasure {
    double real{-1};
    double user{-1};
    double sys{-1};
};

struct TimeStatistics {
    double average{-1};
    double median{-1};
    double min{-1};
    double max{-1};
};

struct SizeStatistics {
    long long int average{-1};
    long long int median{-1};
    long long int min{-1};
    long long int max{-1};
};

void run(const std::string& logDir, const std::vector<Config>& configs);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " log_dir solver" << std::endl;
        return 0;
    }

    std::string logDir(argv[1]);
    std::string solver(argv[2]);

    std::vector<Config> configs;
    configs.emplace_back(4, 4, 4, 1040, solver);
    configs.emplace_back(6, 6, 4, 2330, solver);
    configs.emplace_back(8, 8, 4, 4136, solver);
    configs.emplace_back(12, 12, 4, 9296, solver);
    configs.emplace_back(15, 15, 5, 17873, solver);
    configs.emplace_back(18, 18, 6, 30626, solver);
    configs.emplace_back(24, 24, 8, 71720, solver);
    configs.emplace_back(33, 33, 11, 184544, solver);
    configs.emplace_back(39, 39, 13, 303389, solver);
    configs.emplace_back(54, 54, 18, 800450, solver);
    configs.emplace_back(66, 66, 22, 1457090, solver);
    configs.emplace_back(84, 84, 28, 2995280, solver);
    configs.emplace_back(114, 114, 38, 7466210, solver);
    configs.emplace_back(144, 144, 48, 15023240, solver);
    configs.emplace_back(183, 183, 61, 30792869, solver);
    configs.emplace_back(246, 246, 82, 74707010, solver);
    configs.emplace_back(312, 312, 104, 152294696, solver);

    run(logDir, configs);

    return 0;
}

bool parse_cOMC(const std::string& logDir, const Config& config, double data[2]);
bool parse_cOMCFrontend(const std::string& logDir, const Config& config, double data[2][4]);
bool parse_cMARCOOnly(const std::string& logDir, const Config& config, double data[2][4]);
bool parse_sMARCO(const std::string& logDir, const Config& config, double data[2]);
bool parse_sOMC(const std::string& logDir, const Config& config, double data[2]);
bool parse_csMARCObmodelica(const std::string& logDir, const Config& config, long long int data[4]);
bool parse_csMARCOLLVMIR(const std::string& logDir, const Config& config, long long int data[4]);
bool parse_csMARCOBinary(const std::string& logDir, const Config& config, long long int& data);
bool parse_csOMCC(const std::string& logDir, const Config& config, long long int& data);
bool parse_csOMCBinary(const std::string& logDir, const Config& config, long long int& data);

template<typename T>
void printValue(std::ostream& os, T value) {
    if (value != -1) {
        os << value;
    }

    os << ",";
}

void run(const std::string& logDir, const std::vector<Config>& configs) {
    std::cout << "nx,ny,nz,eq,";

    std::cout << "[c] OMC - real,";
    std::cout << "[c] OMC frontend - real - average,[c] MARCO only - real - average," ;
    std::cout << "[c] OMC frontend - real - median,[c] MARCO only - real - median,";
    std::cout << "[c] OMC frontend - real - min,[c] MARCO only - real - min,";
    std::cout << "[c] OMC frontend - real - max,[c] MARCO only - real - max,";

    std::cout << "[c] OMC - user,";
    std::cout << "[c] OMC frontend - user - average,[c] MARCO only - user - average,";
    std::cout << "[c] OMC frontend - user - median,[c] MARCO only - user - median,";
    std::cout << "[c] OMC frontend - user - min,[c] MARCO only - user - min,";
    std::cout << "[c] OMC frontend - user - max,[c] MARCO only - user - max,";

    std::cout << "[s] MARCO - real,[s] OMC - real,";
    std::cout << "[s] MARCO - user,[s] OMC - user,";

    std::cout << "[cs] MARCO bmodelica - average,[cs] MARCO LLVM-IR - average,";
    std::cout << "[cs] MARCO bmodelica - median,[cs] MARCO LLVM-IR - median,";
    std::cout << "[cs] MARCO bmodelica - min,[cs] MARCO LLVM-IR - min,";
    std::cout << "[cs] MARCO bmodelica - max,[cs] MARCO LLVM-IR - max,";
    std::cout << "[cs] MARCO binary,[cs]OMC C,[cs] OMC binary,";

    std::cout << std::endl;

    for (const Config& config : configs) {
        double cOMC[2];
        double cOMCFrontend[2][4], cMARCOOnly[2][4];
        double sMARCO[2], sOMC[2];
        long long int csMARCObmodelica[4], csMARCOLLVMIR[4], csMARCOBinary, csOMCC, csOMCBinary;

        parse_cOMC(logDir, config, cOMC);
        parse_cOMCFrontend(logDir, config, cOMCFrontend);
        parse_cMARCOOnly(logDir, config, cMARCOOnly);
        parse_sMARCO(logDir, config, sMARCO);
        parse_sOMC(logDir, config, sOMC);
        parse_csMARCObmodelica(logDir, config, csMARCObmodelica);
        parse_csMARCOLLVMIR(logDir, config, csMARCOLLVMIR);
        parse_csMARCOBinary(logDir, config, csMARCOBinary);
        parse_csOMCC(logDir, config, csOMCC);
        parse_csOMCBinary(logDir, config, csOMCBinary);

        std::cout << config.nx << "," << config.ny << "," << config.nz << "," << config.equations << ",";

        printValue(std::cout, cOMC[0]);

        printValue(std::cout, cOMCFrontend[0][0]);
        printValue(std::cout, cMARCOOnly[0][0]);
        printValue(std::cout, cOMCFrontend[0][1]);
        printValue(std::cout, cMARCOOnly[0][1]);
        printValue(std::cout, cOMCFrontend[0][2]);
        printValue(std::cout, cMARCOOnly[0][2]);
        printValue(std::cout, cOMCFrontend[0][3]);
        printValue(std::cout, cMARCOOnly[0][3]);

        printValue(std::cout, cOMC[1]);

        printValue(std::cout, cOMCFrontend[1][0]);
        printValue(std::cout, cMARCOOnly[1][0]);
        printValue(std::cout, cOMCFrontend[1][1]);
        printValue(std::cout, cMARCOOnly[1][1]);
        printValue(std::cout, cOMCFrontend[1][2]);
        printValue(std::cout, cMARCOOnly[1][2]);
        printValue(std::cout, cOMCFrontend[1][3]);
        printValue(std::cout, cMARCOOnly[1][3]);

        printValue(std::cout, sMARCO[0]);
        printValue(std::cout, sOMC[0]);

        printValue(std::cout, sMARCO[1]);
        printValue(std::cout, sOMC[1]);

        printValue(std::cout, csMARCObmodelica[0]);
        printValue(std::cout, csMARCOLLVMIR[0]);
        printValue(std::cout, csMARCObmodelica[1]);
        printValue(std::cout, csMARCOLLVMIR[1]);
        printValue(std::cout, csMARCObmodelica[2]);
        printValue(std::cout, csMARCOLLVMIR[2]);
        printValue(std::cout, csMARCObmodelica[3]);
        printValue(std::cout, csMARCOLLVMIR[3]);

        printValue(std::cout, csMARCOBinary);
        printValue(std::cout, csOMCC);
        printValue(std::cout, csOMCBinary);

        std::cout << std::endl;
    }
}

std::string getConfigString(const Config& config) {
    return std::to_string(config.nx) + "-" + std::to_string(config.ny) + "-" + std::to_string(config.nz) + "-" + config.solver;
}

std::pair<TimeStatistics, TimeStatistics> parseRealUserTimes(FILE* f) {
    std::pair<TimeStatistics, TimeStatistics> result{};

    fscanf(f, " ------");
    fscanf(f, " Real time");
    fscanf(f, " Average: %lf", &result.first.average);
    fscanf(f, " Median: %lf", &result.first.median);
    fscanf(f, " Min: %lf", &result.first.min);
    fscanf(f, " Max: %lf", &result.first.max);
    fscanf(f, " ------");
    fscanf(f, " User time");
    fscanf(f, " Average: %lf", &result.second.average);
    fscanf(f, " Median: %lf", &result.second.median);
    fscanf(f, " Min: %lf", &result.second.min);
    fscanf(f, " Max: %lf", &result.second.max);

    return result;
}

TimeMeasure parseTimeMeasure(FILE* f) {
    TimeMeasure result;

    fscanf(f, " real %lf", &result.real);
    fscanf(f, " user %lf", &result.user);
    fscanf(f, " sys %lf", &result.sys);

    return result;
}

SizeStatistics parseSizeStatistics(FILE* f) {
    SizeStatistics result;

    fscanf(f, " Average: %lld", &result.average);
    fscanf(f, " Median: %lld", &result.median);
    fscanf(f, " Min: %lld", &result.min);
    fscanf(f, " Max: %lld", &result.max);

    return result;
}

bool parse_cOMC(const std::string& logDir, const Config& config, double data[2]) {
    std::string filePath = logDir + "/omc/omc-time_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 2; ++i) {
            data[i] = -1;
        }

        return false;
    }

    auto time = parseTimeMeasure(f);

    data[0] = time.real;
    data[1] = time.user;

    fclose(f);
    return true;
}

bool parse_cOMCFrontend(const std::string& logDir, const Config& config, double data[2][4]) {
    std::string filePath = logDir + "/marco/omc-time_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                data[i][j] = -1;
            }
        }

        return false;
    }

    auto times = parseRealUserTimes(f);

    data[0][0] = times.first.average;
    data[0][1] = times.first.median;
    data[0][2] = times.first.min;
    data[0][3] = times.first.max;

    data[1][0] = times.second.average;
    data[1][1] = times.second.median;
    data[1][2] = times.second.min;
    data[1][3] = times.second.max;

    fclose(f);
    return true;
}

bool parse_cMARCOOnly(const std::string& logDir, const Config& config, double data[2][4]) {
    std::string filePath = logDir + "/marco/marco-compile-time_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                data[i][j] = -1;
            }
        }

        return false;
    }

    auto times = parseRealUserTimes(f);

    data[0][0] = times.first.average;
    data[0][1] = times.first.median;
    data[0][2] = times.first.min;
    data[0][3] = times.first.max;

    data[1][0] = times.second.average;
    data[1][1] = times.second.median;
    data[1][2] = times.second.min;
    data[1][3] = times.second.max;

    fclose(f);
    return true;
}

bool parse_sMARCO(const std::string& logDir, const Config& config, double data[2]) {
    std::string filePath = logDir + "/marco/simulation-time_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 2; ++i) {
            data[i] = -1;
        }

        return false;
    }

    auto time = parseTimeMeasure(f);

    data[0] = time.real;
    data[1] = time.user;

    fclose(f);
    return true;
}

bool parse_sOMC(const std::string& logDir, const Config& config, double data[2]) {
    std::string filePath = logDir + "/omc/simulation-time_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 2; ++i) {
            data[i] = -1;
        }

        return false;
    }

    auto time = parseTimeMeasure(f);

    data[0] = time.real;
    data[1] = time.user;

    fclose(f);
    return true;
}

bool parse_csMARCObmodelica(const std::string& logDir, const Config& config, long long int data[4]) {
    std::string filePath = logDir + "/marco/bmodelica-size_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 4; ++i) {
            data[i] = -1;
        }

        return false;
    }

    auto size = parseSizeStatistics(f);

    data[0] = size.average;
    data[1] = size.median;
    data[2] = size.min;
    data[3] = size.max;

    fclose(f);
    return true;
}

bool parse_csMARCOLLVMIR(const std::string& logDir, const Config& config, long long int data[4]) {
    std::string filePath = logDir + "/marco/llvmir-size_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        for (size_t i = 0; i < 4; ++i) {
            data[i] = -1;
        }

        return false;
    }

    auto size = parseSizeStatistics(f);

    data[0] = size.average;
    data[1] = size.median;
    data[2] = size.min;
    data[3] = size.max;

    fclose(f);
    return true;
}

bool parse_csMARCOBinary(const std::string& logDir, const Config& config, long long int& data) {
    std::string filePath = logDir + "/marco/marco-binary-size_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        data = -1;
        return false;
    }

    fscanf(f, "%lld", &data);
    fclose(f);
    return true;
}

bool parse_csOMCC(const std::string& logDir, const Config& config, long long int& data) {
    std::string filePath = logDir + "/omc/omc-c-size_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        data = -1;
        return false;
    }

    fscanf(f, "%lld", &data);
    fclose(f);
    return true;
}

bool parse_csOMCBinary(const std::string& logDir, const Config& config, long long int& data) {
    std::string filePath = logDir + "/omc/omc-binary-size_" + getConfigString(config) + ".txt";
    FILE* f = fopen(filePath.c_str(), "r");

    if (!f) {
        data = -1;
        return false;
    }

    fscanf(f, "%lld", &data);
    fclose(f);
    return true;
}
