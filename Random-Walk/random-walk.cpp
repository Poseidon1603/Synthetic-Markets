#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <string>
#include "../Utils/utils.h"

using namespace std;
namespace fs = filesystem;

int check_increment_mean(ifstream& f) {
    vector<double> sum;
    string temp;
    cout << "Checking Increment Mean" << '\n';

    // This puts all the increment values into the vector sum
    while (getline(f,temp)) {
        long unsigned index = temp.find(",") + 1;
        if (index == string::npos) break;
        temp = temp.substr(index);
        sum.push_back(stod(temp));
    }

    if (sum.empty()) {
        cout << "Sum vector is empty" << '\n';
    }

    double total = 0;
    // This is the sum of increments
    for (double val : sum) {
        total += val;
    }

    // Returns the mean of increments
    double mean = total / sum.size();
    cout << "Increment mean is " << mean << '\n';
    return !(round(mean));
}

// Takes in a file name and a makes a random walk 
int random_walk(string fileName, string days) {
    string fullFileName = ("../Random-Walk/logs/" + fileName + ".csv");
    fstream file(fullFileName, ios::app);
    double price = 0;
    // for every second we want to generate some data from the pdf function
    double incr_value;
    for (int i = 0; i < (stod(days) * (24 * 60 * 60)); i++) {
        incr_value = random_sample(0,0.08);
        price += incr_value;
        file << price << "," << incr_value << '\n';
    }
    file.close();
    ifstream readFile(fullFileName);
    // Check the mean of the increments
    if (!check_increment_mean(readFile)) {
        cout << "Mean is not approx 0" << '\n';
        return -1;
    }
    return 0;
}
