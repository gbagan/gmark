MATCH (x0)-[:p23|p23*]->(x1), (x0)-[:p24*]->(x2), (x2)-[:p23|p24*]->(x1) RETURN DISTINCT x2, x0, x1;
