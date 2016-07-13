MATCH (x0)-[:p76*]->(x1), (x0)-[:p81|p76*]->(x2), (x1)-[:p54*]->(x3) RETURN DISTINCT x1, x2, x0;
