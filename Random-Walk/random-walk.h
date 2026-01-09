#pragma once 
#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <string>
using namespace std;

// Check the mean of the second item in the file
// Should work with multiple , on the same line
int check_increment_mean(ifstream& f);

// Gets a random sample from a normal distribution
// Use a smaller standard deviation to make the price movement a bit more minimal (more data closer to the mean of 0 is better)
double random_sample(double mean = 0, double stdev = 0.08);

// Given a fileName and a number of days to run the random walk 
// We produce a time series of data
int random_walk(string fileName, string days);

// Given a string, we delete all files in the directory that contain that name
void delete_previous(string filename);