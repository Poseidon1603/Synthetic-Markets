#include <iostream>
#include "../Utils/utils.h"
#include "random-walk.h"


int main(int argc, const char *argv[]) {
    int x;
    if (argc == 1) {
        cout << "Give me how long you want data for pls" << '\n';
    } else if (argc == 2) {
        // If the file already exists delete it
        cout << "Deleting Previous Attempts" << '\n';
        delete_previous("random-walk-log.csv", "../Random-Walk/log/");
        // Start 1 random walk
        cout << "Making 1 random walk for " << argv[1] << " days" << '\n';
        x = random_walk("random-walk-log", argv[1]);
        if (x != 0) {
            return -1;
        }
    } else if (argc == 3) {
        // Delete files if they already exist
        cout << "Deleting Previous Attempts" << '\n';
        delete_previous("random-walk-log-", "../Random-Walk/log/");
        // Start n random walks for m days
        cout << "Making " << argv[2] << " random walks for " << argv[1] << " days" << '\n';
        for (int i = 0; i < stoi(argv[2]); i++) {
            x = random_walk(("random-walk-log-" + to_string(i+1)),argv[1]);
            if (x != 0) {
                return -1;
            }
        }
    } else {
        cout << "Too many arguments chief" << '\n';
    }
    return 0;
}