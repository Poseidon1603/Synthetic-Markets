#include <iostream>
#include <filesystem>
#include <random>


using namespace std;
namespace fs = filesystem;

void delete_previous(string filename) {
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (!entry.is_regular_file()) continue;

        const auto& name = entry.path().filename().string();
        if (name.contains(filename)) {
            remove(entry);
        }
    } 
    return;
}

// Returns a random sample from the gaussian distribution with given parameters
double random_sample(double mean = 0, double stdev = 0.08) {
    static mt19937 rng{random_device{}()};
    normal_distribution<double> dist(mean,stdev);
    return dist(rng);
}