#pragma once

#include <iostream>


using namespace std;

// Produces a stable mean reversion starting at price 0
// You need to provide the mean you want to converge to
// The Speed of the converging into this mean
// And the magnitude of shocks per time step
int stable_mean_reversion(string fileName, int days, double mean, double mr_spead, double s_shocks);

// Returns a vector of prices with a stable mean
vector<double> stable_mean_reversion(int ticks, double mean, double mr_speed, double shocks);

// Produces a drifting mean reversion in the given file
void drifting_mean_reversion(string filename, int ticks, double theta, double sigma, double mean_vol, double mean);

// Drifting mean but it returns a vector of prices instead to be written into a file
vector<double> drifting_mean_reversion(int ticks, double theta, double sigma, double mean_vol, double mean);