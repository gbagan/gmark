MATCH (x0)-[:p54|p76|p54*]->(x1), (x1)-[:p54*]->(x2), (x2)-[:p76*]->(x3) RETURN DISTINCT x0;
