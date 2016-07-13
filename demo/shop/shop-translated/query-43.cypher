MATCH (x0)-[:p76*]->(x1), (x0)-[:p76|p76*]->(x2), (x0)-[:p76|p76|p76*]->(x3) RETURN DISTINCT x0, x1;
