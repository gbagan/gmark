MATCH (x0)-[:p24|p24|p24*]->(x1), (x0)-[:p24*]->(x2), (x0)-[:p24|p24|p24*]->(x3), (x0)-[:p24|p24|p24*]->(x4) RETURN DISTINCT x2, x1, x0, x3;
