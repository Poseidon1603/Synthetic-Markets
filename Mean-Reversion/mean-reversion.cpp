#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "../Utils/utils.h"
using namespace std;

bool check_log_mean(istream& file, double mean) {
    vector<double> sum;
    string temp;
    cout << "Checking Log Mean" << '\n';

    long unsigned end_index;
    while(getline(file,temp)) {
        //0,0,0,0
        end_index = temp.find(",") + 1;
        sum.push_back(stod(temp.substr(end_index)));
    }

    if (sum.empty()) {
        cout << "Sum Vector is empty" << '\n';
        return false;
    }

    double total = 0;

    for (double val : sum) {
        total += val;
    }

    double log_mean = total / sum.size();

    if (round(log_mean * 100) == round(mean * 100)) {
        cout << "Log Mean is Good" << '\n';
        return true;
    }

    cout << "Log mean is cooked" << '\n';
    return false;
}

int stable_mean_reversion(string fileName, int days, double mean, double mr_speed, double shocks) {
    string fullFileName = "../Mean-Reversion/logs/" + fileName + ".csv";
    fstream file(fullFileName, ios::app);
    // Starting price
    double price = 1;
    // Number of ticks to generate
    int ticks = days * 24 * 60 * 60;
    double log_price = log(price);
    // Initial we want to push the starting price of 0 into the file 
    file << price << ",0,0,0" << '\n';
    // Compute the rest of the prices
    double phi = exp(-mr_speed);
    double variance = (shocks * shocks) * (1.0 - phi * phi) / (2.0 * mr_speed);

    for (int i = 0; i < ticks; i++) {
        double z = random_sample(0.0, 1.0);
        log_price = mean + (log_price - mean) * phi
                + sqrt(variance) * z;
        double real_price = exp(log_price);
        file << real_price << "," << log_price << '\n';
    }
    file.close();

    // Now we need to check our values
    ifstream readFile(fullFileName);
    if (!check_log_mean(readFile, mean)) {
        cout << "Log mean is cooked" << '\n';
        return -1;
    }
    return 0;
}


int drifting_mean_reversion(int days) {
    days++;
    return 0;
}