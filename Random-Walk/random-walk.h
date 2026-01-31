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

// Given a fileName and a number of days to run the random walk 
// We produce a time series of data
int random_walk(string fileName, string days);
