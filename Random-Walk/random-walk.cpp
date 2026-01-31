#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <string>
#include "../Utils/utils.h"

using namespace std;
namespace fs = filesystem;

int check_increment_mean(vector<double>& sum) {
    cout << "Checking Increment Mean" << '\n';

    if (sum.empty()) {
        cout << "Sum vector is empty" << '\n';
    }

    double total = 0;
    // This is the sum of increments
    for (double val : sum) {
        total += val;
    }

    // Returns the mean of increments
    double mean = total / sum.size();
    cout << "Increment mean is " << mean << '\n';
    return !(round(mean));
}

// Takes in a file name and a makes a random walk 
int random_walk(string fileName, string days) {
    string fullFileName = ("../Random-Walk/logs/" + fileName + ".csv");
    fstream file(fullFileName, ios::app);
    vector<double> prices;
    vector<double> incr_values;
    double price = 0;

    double incr_value;
    for (int i = 0; i < (stod(days) * (24 * 60 * 60)); i++) {
        incr_value = random_sample(0,0.08);
        incr_values.push_back(incr_value);
        price += incr_value;
        prices.push_back(price);
        file << price << '\n';
    }
    file.close();

    // Check the mean of the increments
    if (!check_increment_mean(incr_values)) {
        cout << "Mean is not approx 0" << '\n';
        return -1;
    }
    return 0;
}
