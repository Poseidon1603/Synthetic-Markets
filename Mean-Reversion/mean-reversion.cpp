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

    long unsigned start_index;
    long unsigned end_index;
    while(getline(file,temp)) {
        //0,0,0,0
        start_index = temp.find("," , (temp.find(",", (temp.find(",") + 1)) + 1)) + 1;
        end_index = temp.find_last_of(",");
        cout << temp.substr(start_index) << '\n';
        temp = temp.substr(start_index, (end_index-start_index));
        cout << temp << '\n';
        sum.push_back(stod(temp));
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
        return true;
    }

    return false;
}

int stable_mean_reversion(string fileName, int days, double mean, double mr_speed, double shocks) {
    string fullFileName = "../Mean-Reversion/logs/" + fileName + ".csv";
    fstream file(fullFileName, ios::app);
    // Starting price
    double price = 1;
    // Number of ticks to generate
    int ticks = days * 24 * 60 * 60;
    double incr_value;
    double real_price;
    double log_price = log(price);
    // Initial we want to push the starting price of 0 into the file 
    file << price << ",0,0,0" << '\n';
    // Compute the rest of the prices
    for (int i = 0; i < ticks; i++) {
        // Work out the increment value
        incr_value = mr_speed * (mean - log_price) + (shocks * random_sample(0,0.08));
        // Get the next price
        log_price += incr_value;
        real_price = exp(log_price);
        // Push the real price into the file first
        // Pushing order: Real Price, Real Increment Value, Log Price, Log Increment Value
        file << real_price << "," << exp(incr_value) << "," << log_price << "," << incr_value << '\n';
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