#include "randomgen.h"

#include <cstdlib>
#include <ostream>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

default_random_engine RANDOM_GEN(chrono::system_clock::now().time_since_epoch().count());

ostream & operator << (ostream& stream, const distribution & dist) {
    if (dist.type == DISTRIBUTION::UNDEFINED) {
        stream << "undefined";
        return stream;
    }
    stream << "(";
    switch(dist.type) {
        case DISTRIBUTION::UNIFORM:
            stream << "uniform";
            break;
        case DISTRIBUTION::ZIPFIAN:
            stream << "zipfian";
            break;
        case DISTRIBUTION::NORMAL:
            stream << "normal";
            break;
        default:
            break;
    }
    stream << " " << dist.arg1 << " " << dist.arg2 << ")";
    return stream;
}

double random_0_1() {
    return uniform_real_distribution<double>(0.0, 1.0)(RANDOM_GEN);
}

random_generator::~random_generator() {}

uniform_random_generator::uniform_random_generator(size_t min, size_t max) {
    uniform_gen = new uniform_int_distribution<size_t>(min, max);
}

uniform_random_generator::~uniform_random_generator() {
    delete uniform_gen;
}
    
size_t uniform_random_generator::next() {
    return (*uniform_gen)(RANDOM_GEN);
}


normal_random_generator::~normal_random_generator() {
    delete normal_gen;
}

size_t normal_random_generator::next() {
    double f = (*normal_gen)(RANDOM_GEN);
    if (f < 0) {
        f = 0;
    }
    size_t n = (size_t) f;
    return n;
}


zipfian_random_generator::zipfian_random_generator(size_t n_, double alpha_, size_t step_) {
    alpha = alpha_;
    n = n_;
    //if (n == 0)
    //    n = 10000;   // TODO
    step = step_;
    real_distribution = new uniform_real_distribution<double>(0.0, 1.0);
    int_gen = new uniform_int_distribution<size_t>(0, step-1);
    c = 0;
    for (size_t i=1; i<=n; i++) {
        c += pow((double) i, -alpha);
    }
    c = 1.0 / c;
}

zipfian_random_generator::~zipfian_random_generator() {
    delete real_distribution;
    delete int_gen;
}
    
size_t zipfian_random_generator::next() {
    double z = (*real_distribution)(RANDOM_GEN);

    double sum_prob = 0;
    size_t val = n;
    for (size_t i=1; i<=n; i++) {
        sum_prob += c * pow((double) i, -alpha);
        if (sum_prob >= z) {
            val = i;
            break;
        }
    }

    if (step == 1) {
        return (val-1);
    }
    else {
        return (val-1) * step + (*int_gen)(RANDOM_GEN);
    }
}

random_generator * make_generator(const distribution & distrib) {
    switch (distrib.type){
        case DISTRIBUTION::UNIFORM:
            return new uniform_random_generator((size_t) distrib.arg1, (size_t) distrib.arg2);
        case DISTRIBUTION::NORMAL:
            return new normal_random_generator(distrib.arg1, distrib.arg2);
        case DISTRIBUTION::ZIPFIAN:
            return new zipfian_random_generator((size_t) distrib.arg1, distrib.arg2);
        case DISTRIBUTION::UNDEFINED:
            return NULL;
    }
    return NULL;
}

random_generator * make_generator_with_mean(DISTRIBUTION::type distrib_type, size_t mean) {
    switch (distrib_type) {
        case DISTRIBUTION::UNIFORM:
            return new uniform_random_generator(0, mean*2);
        case DISTRIBUTION::ZIPFIAN:
            //return new zipfian_random_generator( ) // TODO
            break;
        case DISTRIBUTION::NORMAL:
            return new normal_random_generator(mean, mean/4.0); // TODO
        case DISTRIBUTION::UNDEFINED:
            return NULL;
    }
    return NULL;
}
