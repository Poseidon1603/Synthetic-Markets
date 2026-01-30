#pragma once

#include <iostream>
using namespace std;

// returns nothing, but creates a file with a trending regime in it
void trending_regime_generator(string filename, int ticks, double drift, double sigma, double start_price);