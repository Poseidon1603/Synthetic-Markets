#include <iostream>
#include <random>
#include <fstream>
using namespace std;

double random_sample(double mean = 0, double stdev = 0.5) {
    static mt19937 rng{random_device{}()};
    normal_distribution<double> dist(mean,stdev);
    return dist(rng);
}

// Build a characteristic equation for the random walk 
// We are going to use a gaussian pdf (0,1) to get our data 
double pdf(auto x = 5, auto mean = 0, auto stdev = 1) {
    const double inv_sqrt_2pi = 1.0 / sqrt(2.0 * M_PI);
    double z = (x - mean) / stdev;
    return (inv_sqrt_2pi / stdev) * exp(-0.5 * z * z); 
}

int main(int argc, const char *argv[]) {
    ofstream file("random-walk-log.csv", ios::app);
    double price = 0;
    if (!argc || argc == 1) {
        cout << "Give me how long you want data for pls" << endl;
    } else if (argc == 2) {
        // This is duration in days
       double duration = stod(argv[1]);
       // Now its in seconds
       duration *= (24 * 60 * 60);
       // for every second we want to generate some data from the pdf function
       for (int i = 0; i < duration; i++) {
        price += random_sample(0,0.08);
        file << price << endl;
        cout << "Current Price: " << price << endl;
       }
    } else {
        cout << "Too many arguments chief" << endl;
    }
    return 0;
}

