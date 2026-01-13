#pragma once

#include <iostream>
#include <filesystem>
using namespace std;

// Given a string, we delete all files in the directory that contain that name
void delete_previous(string filename);

// Gets a random sample from a normal distribution
// Use a smaller standard deviation to make the price movement a bit more minimal (more data closer to the mean of 0 is better)
double random_sample(double mean = 0, double stdev = 0.08);