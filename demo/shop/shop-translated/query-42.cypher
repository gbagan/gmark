MATCH (x0)-[:p75]->()<-[:p60]-()<-[:p74]-(x1), (x1)-[:p74|p75*]->(x2), (x2)-[:p75*]->(x3) RETURN DISTINCT x0, x3;
