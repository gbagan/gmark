#include <cstdlib>
#include <iostream>
#include <vector>
#include "querytranslate.h"

int main() {

  qtranslate("../../xml/queries.xml", "a.sql", "a.sparql", "a.cypher", "a.logicblox");
  exit(EXIT_SUCCESS);

}
