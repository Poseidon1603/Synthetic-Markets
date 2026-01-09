#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <filesystem>
#include <string>
#include <iostream>

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
        cout << "Sum vector is empty" << endl;
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

double random_sample(double mean = 0, double stdev = 0.5) {
    static mt19937 rng{random_device{}()};
    normal_distribution<double> dist(mean,stdev);
    return dist(rng);
}

// Takes in a file name and a makes a random walk 
int random_walk(string f, string days) {
    string fileName = ("log/" + f + ".csv");
    fstream file(fileName, ios::app);
    double price = 0;
    // for every second we want to generate some data from the pdf function
    double incr_value;
    for (int i = 0; i < (stod(days) * (24 * 60 * 60)); i++) {
        incr_value = random_sample(0,0.08);
        price += incr_value;
        file << price << "," << incr_value << '\n';
    }

    file.close();

    ifstream readFile(fileName);
    // Check the mean of the increments
    if (!check_increment_mean(readFile)) {
        cout << "Mean is not approx 0" << '\n';
        return -1;
    }
    return 0;
}

// Checks if a file exists
int file_exist(string name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0);
}

void delete_previous(string filename) {
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (!entry.is_regular_file()) continue;

        const auto& name = entry.path().filename().string();
        if (name.contains(filename)) {
            remove(entry);
        }
    } 
    return;
}

int main(int argc, const char *argv[]) {
    if (!argc || argc == 1) {
        cout << "Give me how long you want data for pls" << endl;
    } else if (argc == 2) {
        // If the file already exists delete it
        cout << "Deleting Previous Attempts" << endl;
        delete_previous("random-walk-log.csv");
        // Start 1 random walk
        cout << "Making 1 random walk for " << argv[1] << " days" << endl;
        int  x = random_walk("random-walk-log", argv[1]);
        if (x != 0) {
            return -1;
        }
    } else if (argc == 3) {
        // Delete files if they already exist
        cout << "Deleting Previous Attempts" << '\n';
        delete_previous("random-walk-log-");
        // Start n random walks for m days
        cout << "Making " << argv[2] << " random walks for " << argv[1] << " days" << endl;
        for (int i = 0; i < stoi(argv[2]); i++) {
            int x = random_walk(("random-walk-log-" + to_string(i+1)),argv[1]);
            if (x != 0) {
                return -1;
            }
        }
    } else {
        cout << "Too many arguments chief" << endl;
    }
    return 0;
}

