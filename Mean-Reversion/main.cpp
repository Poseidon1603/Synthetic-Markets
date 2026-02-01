#include <iostream>
#include "../Utils/utils.h"
#include "mean-reversion.h"
using namespace std;


int main(void) {
    string tmp;
    int x;
    int days;
    int runs; 
    char stable_mean;
    double mean;
    double mean_reversion_speed;
    double shocks;
    double mean_vol;
    cout << "Welcome to Mean_Reversion" << '\n';
    cout << "How days worth of ticks would you like? [Type (No Spaces)]" << '\n';
    cin >> days;
    cout << "How many times would you like to run this mean reversion? [Type (No Spaces)]" << '\n';
    cin >> runs;
    cout << "Would like to use a stable mean? [y/n]" << '\n';
    cin >> stable_mean;
    stable_mean = tolower(stable_mean);
    if (stable_mean == 'y') {
        delete_previous("stable-mean-reversion-log", "../Mean-Reversion");
        cout << "What would you like the stable mean to be? [double]" << '\n';
        cin >> mean;
        cout << "What would you like the Mean Reversion Speed to be? [double]" << '\n';
        cin >> mean_reversion_speed;
        cout << "What would you like the Magnitude of random shocks per time step to be? [double]" << '\n';
        cin >> shocks;
        cout << "Running " << runs << " Mean Reversions for " << days << " days" << '\n';
        for (int i = 0; i < runs; i++) {
            tmp = "stable-mean-reversion-log-" + to_string(i+1);
            x = stable_mean_reversion(tmp, (days*24*60*60),mean,mean_reversion_speed,shocks);
            if (x != 0) {
                cout << "Something went wrong" << '\n';
                return -1;
            }
        }
    } else if (stable_mean == 'n') {
        delete_previous("drifting-mean-reversion-log", "../Mean-Reversion");
        cout << "What would you like the initial mean to be? [double]" << '\n';
        cin >> mean; 
        cout << "What would you like the Mean Reversion Speed to be? [double]" << '\n';
        cin >> mean_reversion_speed;
        cout << "What would you like the Magnitude of random shocks per time step to be? [double]" << '\n';
        cin >> shocks;
        cout << "What would you like the mean volatility to be? [How much mean drifts]" << '\n';
        cin >> mean_vol;
        cout << "Running " << runs << " Mean Reversions for " << days << " days" << '\n';
        for (int i = 0; i < runs; i++) {
            tmp = "drifting-mean-reversion-log-" + to_string(i+1);
            drifting_mean_reversion(tmp, (days * 24 * 60 * 60), mean_reversion_speed, shocks, mean_vol, mean);
        }
    } else {
        cout << "Wrong input chief. Try again from the start" << '\n';
        main();
    }
    return 0;
}