#pragma once

#include <iostream>


using namespace std;

// Check the log mean of the provided file
// Make sure the file give contains the following structure 
// Real Price, Real Increment Value, Log Price, Log Increment Value
bool check_log_mean(istream& file, double mean);


// Produces a stable mean reversion starting at price 0
// You need to provide the mean you want to converge to
// The Speed of the converging into this mean
// And the magnitude of shocks per time step
int stable_mean_reversion(string fileName, int days, double mean, double mr_spead, double s_shocks);


// Produces a drifting mean reversion in the given file
void drifting_mean_reversion(string filename, int ticks, double theta, double sigma, double mean_vol, double mean);

double verify_drifting_mean(string fileName);