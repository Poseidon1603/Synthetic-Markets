#include <iostream>
#include "../Utils/utils"

using namespace std;







int main(int argc, const char *argv[]) {
    if (argc == 1) {
        cout << "Give me 1 or 2 arguments please" << '\n';
        cout << "Argument 1 is the time period in days" << '\n';
        cout << "Argument 2 is the number of times to run mean-reversion data generation (defualt 1)" << '\n';
    } else if (argc == 2) {
        cout << "Deleting Previous Attempt" << '\n';
        delete_previous("mean-reversion-log.csv");
        // Do mean reversion here
    } else if (argc == 3) {
        cout << "Deleting Previous Attempts" << '\n';
    }
}