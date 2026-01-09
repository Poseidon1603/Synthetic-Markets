#include <iostream>
#include <random>
#include <fstream>
#include <vector>
using namespace std;

double check_increment_mean(fstream f) {
    vector<double> sum;
    string temp;
    // This puts all the increment values into the vector sum
    while (getline(f,temp)) {
        temp = temp.substr(temp.find(","));
        sum.push_back(stod(temp));
    }

    sum.push_back(0);
    // This is the sum of increments
    for (double val : sum) {
        sum.back() += val;
    }

    // Returns the mean of increments
    return sum.back() / (sum.size() - 1);
}

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
    fstream file("log/random-walk-log.csv", ios::app);
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
        double incr_value = random_sample(0,0.08);
        price += incr_value;
        file << price << "," << incr_value << endl;
       }

       // Check the mean of the increments

       file.close();
    } else {
        cout << "Too many arguments chief" << endl;
    }
    return 0;
}

