#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include "../Utils/utils.h"
using namespace std;


bool verify_variance_stability(const std::vector<double>& residuals) {
    auto calc_variance = [](const std::vector<double>& data, int start, int end) {
        double sum = 0, sq_sum = 0;
        int n = end - start;
        for (int i = start; i < end; ++i) {
            sum += data[i];
            sq_sum += data[i] * data[i];
        }
        double mean = sum / n;
        return (sq_sum / n) - (mean * mean);
    };

    int chunk = residuals.size() * 0.1; // Check first and last 10%
    double var_start = calc_variance(residuals, 0, chunk);
    double var_end = calc_variance(residuals, residuals.size() - chunk, residuals.size());

    cout << "--- Proof 2: Variance Stability ---" << endl;
    cout << "Start Var: " << var_start << " | End Var: " << var_end << endl;
    
    // In a random walk, the ratio would be ~10.0. In MR, it should be ~1.0.
    double ratio = var_end / var_start;
    cout << "Variance Ratio: " << ratio << (ratio < 2.0 ? " (STABLE)" : " (UNSTABLE)") << endl;
    
    return (ratio < 2.0);
}

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

double verify_cointegration(const vector<double>& log_prices, const vector<double>& means) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0, sum_y2 = 0;
    int n = log_prices.size();
    for(int i = 0; i < n; i++) {
        sum_x += log_prices[i]; sum_y += means[i];
        sum_xy += log_prices[i] * means[i];
        sum_x2 += log_prices[i] * log_prices[i];
        sum_y2 += means[i] * means[i];
    }
    double corr = (n * sum_xy - sum_x * sum_y) / sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));
    cout << "--- Proof 3: Cointegration ---" << endl;
    cout << "Correlation: " << corr << (corr > 0.8 ? " (STRONG)" : " (WEAK)") << endl;
    return corr;
}

double verify_drifting_mean(vector<double>& log_prices, vector<double>& means) {
    vector<double> residuals;

    for(size_t i = 0; i < log_prices.size(); ++i) {
        residuals.push_back(log_prices[i] - means[i]);
    }
    
    double beta = verify_reversion(residuals);
    cout << "--- Proof 1: Residual Beta ---" << endl;
    cout << "Residual Beta: " << beta << " (Should be negative)" << endl;
    // Run OLS Beta check on the residuals
    bool var_stable = verify_variance_stability(residuals);
    double corr = verify_cointegration(log_prices, means);

    bool beta_pass = (beta < 0);
    bool corr_pass = (corr > 0.8);

    return (beta_pass && var_stable && corr_pass);
}

bool check_log_mean(vector<double>& prices, double target_mean, double expected_theta) {
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

void stable_mean_reversion(string fileName, int ticks, double mean, double mr_speed, double shocks) {
    string fullFileName = "../Mean-Reversion/logs/" + fileName + ".csv";
    fstream file(fullFileName, ios::app);
    vector<double> log_prices;
    // Starting price
    double price = 1.0;
    // Inital price as a log price
    double log_price = log(price);
    log_prices.push_back(log_price);
    // Initial we want to push the starting price of 0 into the file 
    file <<fixed << price <<  '\n';
    
    // Our phi assumes dt = 1
    double phi = exp(-mr_speed);

    // Check our phi value
    if (phi >= 1) {
        cout << "Phi is greater than 1" << '\n';
        return;
    }

    double variance = sqrt((1 - phi * phi) / (2 * mr_speed));
    
    double z;
    for (int i = 0; i < ticks; i++) {
        z = random_sample(0,1);
        log_price = mean + ((log_price - mean) * phi) + (shocks * variance * z);
        log_prices.push_back(log_price);
        price = exp(log_price);
        file << fixed << price << '\n';
    }
    file.close();

    // Now we need to check our values
    if (!check_log_mean(log_prices, mean, mr_speed)) {
        cout << "Process is cooked" << '\n';
        return;
    }
    return;
}


void drifting_mean_reversion(string filename, int ticks, double theta, double sigma, double mean_vol, double mean) {
    string fullFileName = "../Mean-Reversion/logs/" + filename + ".csv";
    ofstream file(fullFileName, ios::app);
    vector<double> moving_means;
    vector<double> log_prices;
    
    double moving_mean = mean;  // Initial mean
    double log_p = moving_mean; // Start at the mean
    moving_means.push_back(moving_mean);
    log_prices.push_back(log_p);

    double phi = exp(-theta);
    double vol_adj = sqrt((1 - phi * phi) / (2 * theta));

    for (int t = 0; t < ticks; ++t) {
        // Update the Mean itself (Random Walk)
        moving_mean += mean_vol * random_sample(0,1); 
        moving_means.push_back(moving_mean);
        // Update the Price (Mean Reverts to the NEW current mean)
        double z = random_sample(0.0,1.0);
        log_p = moving_mean + (log_p - moving_mean) * phi + (sigma * vol_adj * z);
        log_prices.push_back(log_p);
        
        // current_mean, price , log_price
        file << fixed << exp(log_p) << "\n";
    }
    file.close();

    if (!verify_drifting_mean(log_prices, moving_means)) {
        cout << "Something went wrong" << '\n';
        return;
    }
    
    return; 

}