#include <iostream>
#include "../Utils/utils.h"
#include "trending.h"


int main(void) {
    int ticks;
    int runs;
    double drift = 0.000001;
    double sigma = 0.0002;
    double start_price = 100;
    cout << "Welcome to Trending Markets" << '\n';
    cout << "How long would you like data for? [In Days]" << '\n';
    cin >> ticks;
    ticks *= 24*60*60;
    cout << "How many times would you like to run this? [int]" << '\n';
    cin >> runs;
    cout << "What would you like the drift to be? [double]" << '\n';
    cout << "Drift represents the trend velocity \nIt's the average return percentage per tick" << '\n';
    cin >> drift;
    cout << "What would you like the sigma to be? [double] \n";
    cout << "Sigma represents the volatility or noise of the trend \nIt represents the standard deviation of returns\n";
    cin >> sigma;
    cout << "What would you like the start price to be? [double] \n";
    cin >> start_price;
    cout << "Deleting Previous Attempts" << '\n';
    delete_previous("trending-log-", "../Trending");
    cout << "Starting " << runs << " for " << ticks << " ticks each" << '\n';
    for (int i = 0; i < runs; ++i) {
        trending_regime_generator(("trending-log-" + i),ticks,drift,sigma, start_price);
    }
    return 0;
}