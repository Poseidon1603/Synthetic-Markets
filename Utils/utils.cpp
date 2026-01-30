#include <iostream>
#include <filesystem>
#include <random>


using namespace std;
namespace fs = filesystem;

void delete_previous(string filename, string directory) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
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
// Calculate variance of given data and period
double calc_variance(const std::vector<double>& data, int start, int end) {
    double sum = 0, sq_sum = 0;
    int n = end - start;
    for (int i = start; i < end; ++i) {
        sum += data[i];
        sq_sum += data[i] * data[i];
    }
    double mean = sum / n;
    return (sq_sum / n) - (mean * mean);
}