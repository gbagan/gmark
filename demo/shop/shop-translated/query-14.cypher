MATCH (x0)-[:p81|p81*]->(x1), (x1)-[:p54]->()<-[:p58]-()<-[:p72]-(x2), (x2)-[:p81*]->(x3) RETURN DISTINCT x0, x3;
