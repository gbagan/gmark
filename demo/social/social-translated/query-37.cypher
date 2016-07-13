MATCH (x0)-[:p15*]->(x1), (x0)-[:p18*]->(x2), (x0)-[:p15*]->(x3), (x0)-[:p18|p15*]->(x4) RETURN DISTINCT x2, x1, x0;
