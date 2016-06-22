#ifndef GMARK_REPORT_H
#define GMARK_REPORT_H

#include "randomgen.h"
#include <iostream>

namespace report {

using namespace std;

class report {
public:
    size_t nb_nodes;
    size_t nb_edges;
    double exec_time;
};
}

#endif
