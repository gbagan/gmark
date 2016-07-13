MATCH (x0)-[:p54|p65*]->(x1), (x0)-[:p76|p65|p54*]->(x2), (x1)-[:p65|p65|p76*]->(x3) RETURN DISTINCT x0, x1;
