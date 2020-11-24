#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <numeric>
#include <array>
#include <memory>
#include <functional>
#include <set>
#include <list>
#include <map>
#include <chrono>
#include <numeric>
#include <fstream>

using namespace std;

vector<double> generate_random_x(int n, int min, int max) { //drugi X to Y

    vector<double> ret;
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(min, max);

    for(int i = 0; i < n; i++) {
        ret.push_back(distribution(generator));
    }
    return ret;
}
    
auto best = [](auto function, int n, int iterations = 1000, double minDomain = -10, double maxDomain = 10) { 

    double min = minDomain;
    double max = maxDomain;

    vector<double> best_x;

    if (n > 1) {
        double solution = function(generate_random_x(n, minDomain, maxDomain));
        for (int i = 0; i < iterations; i++) {
            vector<double> random_x = generate_random_x(n, minDomain, maxDomain);
            double newSolution = function(random_x);
            if (newSolution < solution) { //minimalizacja
                best_x = random_x;
                solution = newSolution;
            }
        }
    }
    return best_x;
};

int main() {

    int min, max, iterations;

    auto matyas = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return 0.26 * ((x * x) + (y *  y) - (0.48 * x * y));
    };

    auto himmelblau = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return (pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2.0));
    };

    auto mccormick = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return (sin(x + y) + pow(x - y, 2.0) - 1.5 * x + 2.5 * y + 1);
    };

    auto sphere = [](vector<double> x) { //funkcja tylko z X
        double sum = 0;
        for (auto e : x) { // podmienia listę x na jeden double
            sum += e * e;
        }
        return sum;
    };

    auto function = matyas;

    vector<double> bestPoints = best(function, 2);

    cout << "The best point: " << endl;
    
    cout << "X = " << bestPoints.at(0) << " Y = " << bestPoints.at(1) << endl;
}