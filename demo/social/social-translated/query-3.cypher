MATCH (x0)-[:p24*]->(x1), (x0)-[:p25|p25*]->(x2), (x0)-[:p25*]->(x3) RETURN DISTINCT x0, x2, x3, x1;
