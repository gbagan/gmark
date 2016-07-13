MATCH (x0)-[:p14]->()<-[:p8]-()<-[:p0]-(x1), (x1)-[:p21|p17|p16*]->(x2), (x2)-[:p21|p16|p21*]->(x3) RETURN DISTINCT x0, x3;
