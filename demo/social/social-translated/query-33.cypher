MATCH (x0)-[:p16*]->(x1), (x1)-[:p16|p16|p16*]->(x2), (x0)-[:p6|p7*]->(x3), (x2)-[:p16|p7|p6*]->(x4) RETURN DISTINCT x2, x1, x0;
