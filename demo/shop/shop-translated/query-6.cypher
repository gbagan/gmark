MATCH (x0)-[:p54|p79|p54*]->(x1), (x0)-[:p72|p72*]->(x2), (x2)-[:p71|p79*]->(x1) RETURN DISTINCT x1, x0, x2;
