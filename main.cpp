#include <iostream>
#include <cstdlib>

#include "driver.hpp"

using namespace std;

int main(const int argc, const char **argv) {
    if (argc != 2) {
        cout << "Not enough args" << endl;
        return (EXIT_FAILURE);
    }
    ECMA::Driver driver;

    driver.parse(argv[1]);

    cout << "Done!" <<endl;

    return (EXIT_SUCCESS);
}
