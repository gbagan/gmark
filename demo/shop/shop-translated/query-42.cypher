MATCH (x0)-[:p72|p54|p71*]->(x1), (x0)-[:p54|p76|p72*]->(x2), (x0)-[:p72|p54*]->(x3) RETURN DISTINCT x1, x0, x2;
