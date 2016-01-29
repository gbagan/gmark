/* gMark
 * Copyright (C) 2015-2016 Guillaume Bagan <guillaume.bagan@liris.cnrs.fr>
 * Copyright (C) 2015-2016 Angela Bonifati <angela.bonifati@univ-lyon1.fr>
 * Copyright (C) 2015-2016 Radu Ciucanu <radu.ciucanu@cs.ox.ac.uk>
 * Copyright (C) 2015-2016 George Fletcher <g.h.l.fletcher@tue.nl>
 * Copyright (C) 2015-2016 Aurélien Lemay <aurelien.lemay@univ-lille3.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

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
