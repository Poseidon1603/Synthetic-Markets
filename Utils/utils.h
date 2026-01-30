#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;

// Given a string, we delete all files in the directory that contain that name
// Directory is relative to /build
void delete_previous(string filename, string directory);

// Gets a random sample from a normal distribution
// Use a smaller standard deviation to make the price movement a bit more minimal (more data closer to the mean of 0 is better)
double random_sample(double mean, double stdev);

// Returns the variance of some data
double calc_variance(const std::vector<double>& data, int start, int end);