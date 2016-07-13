MATCH (x0)-[:p24*]->(x1), (x1)<-[:p24]-()-[:p13]->()<-[:p13]-(x2), (x2)-[:p25*]->(x3) RETURN DISTINCT x2, x1, x0;
