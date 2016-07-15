MATCH (x0)-[:p25*]->(x1), (x0)-[:p25*]->(x2), (x0)-[:p25|p25*]->(x3) RETURN DISTINCT x3, x0, x1, x2;
