#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "../Utils/utils.h"
using namespace std;

double verify_reversion(const std::vector<double>& path) {
    double sum_x = 0, sum_dx = 0, sum_x_sq = 0, sum_x_dx = 0;
    int n = path.size() - 1;

    for (int i = 0; i < n; ++i) {
        double x_prev = path[i];
        double dx = path[i+1] - path[i];

        sum_x += x_prev;
        sum_dx += dx;
        sum_x_sq += x_prev * x_prev;
        sum_x_dx += x_prev * dx;
    }

    // Solve for Beta using OLS: (n*Σ(xy) - Σx*Σy) / (n*Σ(x^2) - (Σx)^2)
    double beta = (n * sum_x_dx - sum_x * sum_dx) / (n * sum_x_sq - sum_x * sum_x);
    return beta;
}

double verify_drifting_mean(string fileName) {
    vector<double> log_prices; 
    vector<double> means; 
    vector<double> residuals;

    string path = "../Mean-Reversion/logs/" + fileName + ".csv";
    ifstream file(path);
    string line;
    int first_comma;
    int last_comma;
    while (getline(file,line)) {
        first_comma = line.find(",") + 1;
        last_comma = line.find_last_of(",") + 1;
        log_prices.push_back(stod(line.substr(last_comma)));
        means.push_back(stod(line.substr(0, first_comma)));
    }
    file.close();

    for(size_t i = 0; i < log_prices.size(); ++i) {
        residuals.push_back(log_prices[i] - means[i]);
    }

    // Run OLS Beta check on the residuals
    double beta = verify_reversion(residuals);
    
    cout << "--- Proof 1: Residual Beta ---" << endl;
    cout << "Residual Beta: " << beta << " (Should be negative)" << endl;
    return beta;
}

bool check_log_mean(istream& file, double target_mean, double expected_theta) {
    vector<double> prices;
    string line;
    
    // Parse CSV
    while(getline(file, line)) {
        size_t comma_pos = line.find(",");
        if (comma_pos != string::npos) {
            prices.push_back(stod(line.substr(comma_pos + 1)));
        }
    }

    // Calculate Empirical Mean
    double total = 0;
    for (double p : prices) total += p;
    double empirical_mean = total / prices.size();

    // Calculate Beta (using verify_reversion)
    double beta = verify_reversion(prices);
    

    // Expected beta for dt=1 is: exp(-theta) - 1
    double theoretical_beta = exp(-expected_theta) - 1;

    cout << "--- CHECKS ---" << endl;
    cout << "Target Mean: " << target_mean << " | Empirical: " << empirical_mean << endl;
    cout << "Expected Beta: " << theoretical_beta << " | Empirical: " << beta << endl;

    // We check if the beta is at least negative and within a reasonable distance of theory
    bool mean_ok = std::abs(empirical_mean - target_mean) < 0.01; // Loosen tolerance slightly
    bool beta_ok = (beta < 0) && (std::abs(beta - theoretical_beta) < 0.1);

    return mean_ok && beta_ok;
}

int stable_mean_reversion(string fileName, int ticks, double mean, double mr_speed, double shocks) {
    string fullFileName = "../Mean-Reversion/logs/" + fileName + ".csv";
    fstream file(fullFileName, ios::app);
    // Starting price
    double price = 1.0;
    // Inital price as a log price
    double log_price = log(price);
    // Initial we want to push the starting price of 0 into the file 
    file << price << "," << log_price << '\n';
    
    // Our phi assumes dt = 1
    double phi = exp(-mr_speed);

    // Check our phi value
    if (phi >= 1) {
        cout << "Phi is greater than 1" << '\n';
        return false;
    }

    double variance = sqrt((1 - phi * phi) / (2 * mr_speed));
    
    double z;
    for (int i = 0; i < ticks; i++) {
        z = random_sample(0,1);
        log_price = mean + ((log_price - mean) * phi) + (shocks * variance * z);
        price = exp(log_price);
        file << price << "," << log_price << '\n';
    }
    file.close();

    // Now we need to check our values
    ifstream readFile(fullFileName);
    if (!check_log_mean(readFile, mean, mr_speed)) {
        cout << "Process is cooked" << '\n';
        return -1;
    }
    return 0;
}


void drifting_mean_reversion(string filename, int ticks, double theta, double sigma, double mean_vol, double mean) {
    string fullFileName = "../Mean-Reversion/logs/" + filename + ".csv";
    ofstream file(fullFileName, ios::app);

    
    double moving_mean = mean;  // Initial mean
    double log_p = moving_mean; // Start at the mean
    
    double phi = exp(-theta);
    double vol_adj = sqrt((1 - phi * phi) / (2 * theta));

    for (int t = 0; t < ticks; ++t) {
        // Update the Mean itself (Random Walk)
        moving_mean += mean_vol * random_sample(0,1); 

        // Update the Price (Mean Reverts to the NEW current mean)
        double z = random_sample(0.0,1.0);
        log_p = moving_mean + (log_p - moving_mean) * phi + (sigma * vol_adj * z);
        
        // current_mean, price , log_price
        file << moving_mean << "," << exp(log_p) << "," << log_p << "\n";
    }
    file.close();

    cout << "verifying drifting mean called" << '\n';
    if (verify_drifting_mean(filename) < 0) {
        return;
    }

    cout << "Something went wrong" << '\n';
}