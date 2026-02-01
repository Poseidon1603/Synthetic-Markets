#include <iostream>
#include <filesystem>
#include <fstream>
#include <math.h>
#include "../Utils/utils.h"
#include <vector>

using namespace std;

double test_momentum_correlation(const std::vector<double>& log_prices) {
    std::vector<double> returns;
    for (size_t i = 1; i < log_prices.size(); ++i) {
        returns.push_back(log_prices[i] - log_prices[i-1]);
    }

    // Correlate returns[t] with returns[t-1]
    double sum_r1 = 0, sum_r2 = 0, sum_r1r2 = 0, sum_r1_sq = 0, sum_r2_sq = 0;
    int n = returns.size() - 1;

    for (int i = 0; i < n; ++i) {
        double r1 = returns[i];
        double r2 = returns[i+1];
        sum_r1 += r1; sum_r2 += r2;
        sum_r1r2 += r1 * r2;
        sum_r1_sq += r1 * r1; sum_r2_sq += r2 * r2;
    }

    return (n * sum_r1r2 - sum_r1 * sum_r2) / 
           std::sqrt((n * sum_r1_sq - sum_r1 * sum_r1) * (n * sum_r2_sq - sum_r2 * sum_r2));
}


double test_displacement_ratio(const std::vector<double>& log_prices) {
    double total_move = std::abs(log_prices.back() - log_prices.front());
    
    // Calculate standard deviation of returns (shocks)
    double sum_sq_diff = 0;
    for (size_t i = 1; i < log_prices.size(); ++i) {
        double diff = log_prices[i] - log_prices[i-1];
        sum_sq_diff += diff * diff;
    }
    double stdev = std::sqrt(sum_sq_diff / log_prices.size());

    // How many 'stdevs' did the price move from start to finish?
    return total_move / stdev;
}

double test_variance_expansion(const std::vector<double>& log_prices) {
    int n = log_prices.size();
    int chunk = n * 0.1; // 10% sample

    double var_initial = calc_variance(log_prices, 0, chunk);
    double var_total = calc_variance(log_prices, 0, n); // Variance of the whole series

    // Ratio of total variance to initial noise
    return var_total / var_initial;
}

double test_beta_zero(const std::vector<double>& log_prices) {
    double sum_x = 0, sum_dx = 0, sum_x_sq = 0, sum_x_dx = 0;
    int n = log_prices.size() - 1;

    for (int i = 0; i < n; ++i) {
        double x_prev = log_prices[i];
        double dx = log_prices[i+1] - log_prices[i];
        sum_x += x_prev;
        sum_dx += dx;
        sum_x_sq += x_prev * x_prev;
        sum_x_dx += x_prev * dx;
    }
    // OLS Slope
    return (n * sum_x_dx - sum_x * sum_dx) / (n * sum_x_sq - sum_x * sum_x);
}



bool check_trend(vector<double>& log_prices) {
    double beta = test_beta_zero(log_prices);
    double ratio = test_variance_expansion(log_prices);
    double displacement = test_displacement_ratio(log_prices);
    double corr = test_momentum_correlation(log_prices);

    if ((round(beta * 100) > 1  || round(beta * 100) < -1 )) {
        cout << "Beta Test Fails \n";
        return false;
    }

    if (ratio < 5) {
        cout << "Variance test fails \n";
        cout << "Var is " << ratio << '\n';
        return false;
    }

    if (displacement < 1) {
        cout << "Displacement Test fails \n";
        cout << "Displacement is " << displacement << '\n';
        return false;
    }

    if (round(corr * 100) > 1  || round(corr * 100) < -1 ) {
        cout << "Auto Correlation test fails \n";
        cout << "Corr is " << corr << '\n';
        return false;
    }


    return true;
}

vector<double> trending_regime_generator(int ticks, double drift, double sigma, double start_price) {
    vector<double> prices;
    vector<double> log_prices;

    double log_p = log(start_price);
    log_prices.push_back(log_p);
    prices.push_back(start_price);

    double log_drift = drift - (0.5 * sigma * sigma);

    double z = 0;
    for ( int t = 0; t < ticks; ++t) {
        z = random_sample(0,1);

        log_p = log_drift + (sigma * z);
        log_prices.push_back(log_p);

        prices.push_back(exp(log_p));
    }

    if (!check_trend(log_prices)) {
        cout << "Something went wrong \n";
        vector<double> empty = {};
        return empty;
    }

    return prices;
}

void trending_regime_generator(string filename, int ticks, double drift, double sigma, double start_price) {
    string fullFileName = "../Trending/logs/" + filename + ".csv";
    ofstream file(fullFileName, ios::app);
    vector<double> log_prices;

    double log_p = log(start_price);
    log_prices.push_back(log_p);
    
    // For GBM, the expected log price change is (drift - 0.5 * sigma^2)
    double log_drift = drift - (0.5 * sigma * sigma);

    for (int t = 0; t < ticks; ++t) {
        double z = random_sample(0.0, 1.0);
        
        // The core GBM equation in log-space
        // log_p_t = log_p_{t-1} + drift + noise
        log_p += log_drift + (sigma * z);
        log_prices.push_back(log_p);
        
        // time, price, log_price
        file << exp(log_p) << '\n';
    }
    file.close();

    cout << "Running Checks on Output \n";

    if (!check_trend(log_prices)) {
        return;
    }

    cout << "All Checks Pass \n";
    return;
}