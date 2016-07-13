MATCH (x0)-[:p72|p81|p54*]->(x1), (x0)-[:p81|p72|p54*]->(x2), (x0)-[:p76|p72*]->(x3), (x0)-[:p54*]->(x4) RETURN DISTINCT x1, x0, x3, x2;
