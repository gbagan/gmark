MATCH (x0)-[:p54|p54*]->(x1), (x0)-[:p58|p54*]->(x2), (x1)-[:p58*]->(x3) RETURN DISTINCT x2, x0, x3, x1;
