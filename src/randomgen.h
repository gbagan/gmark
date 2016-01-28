#ifndef RANDOMGEN_H
#define RANDOMGEN_H

#include <random>

using namespace std;

namespace DISTRIBUTION {
    enum type {
        UNIFORM,
        NORMAL,
        ZIPFIAN,
        UNDEFINED
    };
};

class distribution {
public:
    DISTRIBUTION::type type;
    double arg1;
    double arg2;
    
    distribution() {
        type = DISTRIBUTION::UNDEFINED;
        arg1 = 0.0;
        arg2 = 0.0;
    }
 
    distribution(DISTRIBUTION::type t, double a1, double a2) {
        type = t;
        arg1 = a1;
        arg2 = a2;
    }
    

    distribution(const distribution & o) {
        type = o.type;
        arg1 = o.arg1;
        arg2 = o.arg2;
    }
    
    distribution & operator=(const distribution & o) {
        type = o.type;
        arg1 = o.arg1;
        arg2 = o.arg2;
        return *this;
    }
};

ostream& operator<< (ostream& stream, const distribution & dist);

double random_0_1();


class random_generator {
public:
    virtual ~random_generator() = 0;
    virtual size_t next() = 0;
};

class uniform_random_generator : public random_generator {
private:
    uniform_int_distribution<size_t> * uniform_gen;
    
public:
    uniform_random_generator(size_t min, size_t max);
    ~uniform_random_generator();
    size_t next();
};

class normal_random_generator : public random_generator {
private:
    normal_distribution<double> * normal_gen = NULL;
    
public:
    normal_random_generator(double mu, double sigma) {
        normal_gen = new normal_distribution<double>(mu, sigma);
    }
    
    ~normal_random_generator();
    size_t next();
};

class zipfian_random_generator : public random_generator {
private:
    double alpha;
    double c;
    size_t n;
    size_t step;
    uniform_int_distribution<size_t> * int_gen;
    uniform_real_distribution<double> * real_distribution;

public:
    zipfian_random_generator(size_t n_, double alpha_=2.5, size_t step_=1);
    ~zipfian_random_generator();
    size_t next();
};

random_generator * make_generator(const distribution & distrib);
random_generator * make_generator_with_mean(DISTRIBUTION::type distrib_type, size_t mean);

#endif