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

void save_plot(vector<double> sTimes, string name) {

    ofstream file(name + ".txt");
    if (file.is_open()) {
        for(int i = 0; i < sTimes.size(); i++) {
            file << fixed << (i + 1) << " " << sTimes.at(i) << endl; //fixed -> zamiast E liczby
        }
        file.close();
    }
}

void save_plot_avg(vector<vector<double>> sTimes, string name) {

    vector<double> sTimesAvg;

    for (int j = 0; j < sTimes.at(0).size(); j++) {
        double avg = 0;
        for(int i = 0; i < sTimes.size(); i++) {
            avg += sTimes.at(i).at(j);
        }
        avg /= sTimes.size();
        sTimesAvg.push_back(avg);
    }
    save_plot(sTimesAvg, (name + "2"));
}

vector<double> generate_random_x(int n, int min, int max) { //drugi X to Y

    vector<double> gen;
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(min, max);

    for(int i = 0; i < n; i++) {
        gen.push_back(distribution(generator));
    }
    return gen;
}

vector<double> generate_random_neigh(vector<double> x) {

    vector<double> gen;
    random_device device;
    mt19937 generator(device());

    for(int i = 0; i < x.size(); i++) {
        uniform_real_distribution<double> distribution(x.at(i)-0.1, x.at(i)+0.1);
        gen.push_back(distribution(generator));
    }
    return gen;
}
    
auto hillclimbing = [](auto function, vector<double> & solutionTime, int n = 10, int iterations = 1000, double minDomain = -10, double maxDomain = 10) {

    double min = minDomain;
    double max = maxDomain;

    vector<double> best_x;

    vector<double> random_x = generate_random_x(n, minDomain, maxDomain);
    double solution = function(random_x);
    for (int i = 0; i < iterations; i++) {
        random_x = generate_random_neigh(random_x);
        double newSolution = function(random_x);
        if (newSolution < solution) { //minimalizacja
            best_x = random_x;
            solution = newSolution;
        }
        solutionTime.push_back(solution);
    }
    return best_x;
};

map<string, string> process_args(int argc, char **argv) {

    map<string, string> args;
    string argname = "";
    for (auto arg : vector<string>(argv + 1, argv + argc)) {
        if (arg.size() && arg[0] == '-') {
            argname = arg;
        } else {
            args[argname] = arg;
        }
    }
    return args;
}

int main(int argc, char **argv) {

    map<string, string> args = process_args(argc, argv);

    vector<double> best_x;

    int iterations = 1000;
    int n = 10;
    bool tests = false;

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

    auto sphere = [](vector<double> x) { //funkcja tylko z X
        double sum = 0;
        for (auto e : x) { // podmienia listę x na jeden double
            sum += e * e;
        }
        return sum;
    };
    
    if (args.find("-n") != args.end()) n = stoi(args["-n"]);
    if (args.find("-i") != args.end()) iterations = stoi(args["-i"]);
    if (args.find("-t") != args.end()) tests = (bool)stoi(args["-t"]);

    if (args.find("-f") != args.end()) {
        if (args["-f"] == "matyas") {
            vector<double> solutionTime;
            if(tests) {
                vector<vector<double>> solutionTimes;
                for(int i = 0; i < 5; i++) { //5 eksperymentów
                    best_x = hillclimbing(matyas, solutionTime, 2, 100, -10, 10);
                    solutionTimes.push_back(solutionTime);
                    solutionTime.clear();
                }
                save_plot_avg(solutionTimes, "matyas");
            } else {
                best_x = hillclimbing(matyas, solutionTime, n, iterations, -10, 10);
                save_plot(solutionTime, "matyas");
            }
            cout << "The best points: " << endl;
            cout << "X = " << best_x.at(0) << " Y = " << best_x.at(1) << endl;
        }
        if (args["-f"] == "himmelblau") {
            vector<double> solutionTime;
            if(tests) {
                vector<vector<double>> solutionTimes;
                for(int i = 0; i < 5; i++) {
                    best_x = hillclimbing(himmelblau, solutionTime, n, iterations, -5, 5);
                    solutionTimes.push_back(solutionTime);
                    solutionTime.clear();
                }
                save_plot_avg(solutionTimes, "himmelblau");
            } else {
                best_x = hillclimbing(himmelblau, solutionTime, n, iterations, -5, 5);
                save_plot(solutionTime, "himmelblau");
            }
            cout << "The best points: " << endl;
            cout << "X = " << best_x.at(0) << " Y = " << best_x.at(1) << endl;
        }
        if (args["-f"] == "sphere") {
            vector<double> solutionTime;
            if(tests) {
                vector<vector<double>> solutionTimes;
                for(int i = 0; i < 5; i++) {
                    best_x = hillclimbing(sphere, solutionTime, n, iterations);
                    solutionTimes.push_back(solutionTime);
                    solutionTime.clear();
                }
                save_plot_avg(solutionTimes, "sphere");
            } else {
                best_x = hillclimbing(sphere, solutionTime, n, iterations);
                save_plot(solutionTime, "sphere");
            }
            cout << "The best Xs: " << endl;
            for (int i = 0; i < best_x.size(); i++) {
            cout << "x[" << (i+1) << "] = " << best_x.at(i) << endl;
            }
        }
    }
}