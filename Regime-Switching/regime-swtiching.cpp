#include <iostream>
#include <vector>
#include <math.h>
#include <filesystem>
#include <fstream>
#include "../Utils/utils.h"

using namespace std;



enum MarketState { MEAN_REVERTING = 0, TRENDING = 1, RANDOM_WALK = 2 };

void generate_multi_regime_market(string filename, int total_ticks) {
    ofstream data_file("../Mean-Reversion/logs/" + filename + ".csv", ios::app);
    ofstream log_file("../Mean-Reversion/logs/" + filename + "_metadata.txt", ios::app);

    // Start at a neutral price
    double log_p = log(100.0);
    MarketState state = MEAN_REVERTING;
    
    // Params
    double theta = 0.1, sigma_mr = 0.01, target_mean = log_p;
    double drift = 0.000002, sigma_trend = 0.0002;
    double sigma_rw = 0.0005; // Random walk noise
    
    // 0.015% chance to switch (slightly higher now that we have 3 states)
    double p_switch = 0.00015; 

    for (int t = 0; t < total_ticks; ++t) {
        if (random_sample(0.0, 1.0) < p_switch) {
            // Pick a new state that isn't the current one
            int next_state = rand() % 3;
            while(next_state == (int)state) next_state = rand() % 3;
            state = (MarketState)next_state;

            string state_name;
            if (state == MEAN_REVERTING) {
                state_name = "MEAN_REVERTING";
                target_mean = log_p; // Set mean to current price to avoid "jumping"
            } else if (state == TRENDING) {
                state_name = "TRENDING";
                drift = (random_sample(0, 1) > 0.5) ? 0.000002 : -0.000002;
            } else {
                state_name = "RANDOM_WALK";
            }

            log_file << state << "," << state_name << "," << exp(log_p) << "\n";
            cout << "[Tick " << t << "] New Regime: " << state_name << endl;
        }

        double z = random_sample(0.0, 1.0);
        
        switch(state) {
            case MEAN_REVERTING: {
                double phi = exp(-theta);
                double vol_adj = sqrt((1 - phi * phi) / (2 * theta));
                log_p = target_mean + (log_p - target_mean) * phi + (sigma_mr * vol_adj * z);
                break;
            }
            case TRENDING: {
                log_p += drift + (sigma_trend * z);
                break;
            }
            case RANDOM_WALK: {
                // Pure Random Walk
                log_p += (sigma_rw * z);
                break;
            }
        }

        data_file << exp(log_p) << "," << log_p << "\n";
    }

    data_file.close();
    log_file.close();
}