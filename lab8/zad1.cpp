#include <vector>
#include <random>
#include <functional>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

static int GENE_LENGTH = 128;
static double VALUE_MAX = 0.184467; //64 jedynki bez 20 zer
static double VALUE_MULTIPLER = 54.21; // value - max/2 * multiplier -> górna dziedzina
static double VALUE_DIVIDER = pow(10, 20);

using chromosome_t = vector<int>;

struct point_t {
    public:
    double x,y;

    point_t(double x, double y) {
        this->x = x;
        this->y = y;
    };
};

//PHENOTYPE AND GENOTYPE
chromosome_t generate_random_genotype(int n = GENE_LENGTH) { //generowanie losowych 128 bool
    
    chromosome_t ret;
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(0,1);
    for (int i = 0; i < n; i++) {
        ret.push_back(distribution(generator));
    }
    return ret;
}

chromosome_t phen_to_gen(point_t point) {
    chromosome_t genotype;
    int bin[GENE_LENGTH];
    unsigned long long int x = ((point.x / VALUE_MULTIPLER) + (VALUE_MAX / 2)) * VALUE_DIVIDER;
    unsigned long long int y = ((point.y / VALUE_MULTIPLER) + (VALUE_MAX / 2)) * VALUE_DIVIDER;
  
    for(int i = GENE_LENGTH / 2; i > 0; i--) {
        bin[i - 1] = x % 2; 
        x = x / 2; 
    }
    for(int i = GENE_LENGTH; i > GENE_LENGTH / 2; i--) {
        bin[i - 1] = y % 2; 
        y = y / 2; 
    }
    for(int i = 0; i < GENE_LENGTH / 2; i++) {
        genotype.push_back(bin[i]);
    }
        for(int i = GENE_LENGTH / 2; i < GENE_LENGTH; i++) {
        genotype.push_back(bin[i]);
    }
    return genotype;
}

point_t gen_to_phen(chromosome_t genotype) {  // dzieli gen na połowę i zmienia na phen
    point_t point(0,0);
    int genotype_center = GENE_LENGTH/2;

    for(int i = genotype_center; i > 0; i--) {
        point.x += genotype.at(i-1) * pow(2, genotype_center - i);
    }
    for(int i = genotype.size(); i > genotype_center; i--) {
        point.y += genotype.at(i-1) * pow(2, GENE_LENGTH - i);
    }
    point.x = (point.x / VALUE_DIVIDER - VALUE_MAX / 2) * VALUE_MULTIPLER;  //mieści się w dziedzinie
    point.y = (point.y / VALUE_DIVIDER - VALUE_MAX / 2) * VALUE_MULTIPLER;
    return point;
}

//FITNESS

double calc_fitness(auto function, int x, int y) {
  return abs(function(x,y));
}

void print_genotype(chromosome_t genotype) {
    for(int i = 0; i < genotype.size(); i++) {
        cout << genotype.at(i) << " "; 
    }
    cout << endl;
}

auto best = [](auto function, int iterations = 1000) {
    point_t best_point(0,0);

    point_t point = gen_to_phen(generate_random_genotype());
    double fitness = calc_fitness(function, point.x, point.y);
    for (int i = 0; i < iterations; i++) {
        point = gen_to_phen(generate_random_genotype());
        double new_fitness = calc_fitness(function, point.x, point.y);
        if (new_fitness > fitness)  
        {
            best_point = point;
            fitness = new_fitness;
            cout << "new the best phenotype[" << i << "]: X:" <<  best_point.x << " Y:" << best_point.y << " Fitness:" << new_fitness << endl;
        }
    }
    return best_point;
};

//MAIN

int main() {
    point_t point(0,0);

    auto himmelblau = [](double x, double y) {
        return pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2.0);
    };

    point = best(himmelblau);
    cout << "X: " << point.x << " Y: " << point.y << endl;

    //CONVERSION 

    /*auto genotype = generate_random_genotype();
    print_genotype(genotype);
    point = gen_to_phen(genotype);
    genotype = phen_to_gen(point);
    print_genotype(genotype);*/
    
}