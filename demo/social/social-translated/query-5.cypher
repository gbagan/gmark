MATCH (x0)-[:p25|p25*]->(x1), (x1)<-[:p10]-()-[:p26]->()<-[:p16]-()<-[:p7]-(x2), (x2)-[:p16*]->(x3) RETURN DISTINCT x3, x2, x1, x0 UNION ;
