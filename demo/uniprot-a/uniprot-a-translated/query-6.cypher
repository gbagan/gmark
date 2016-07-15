MATCH (x0)-[:pInteracts*]->(x1), (x0)-[:pInteracts*]->(x2), (x0)-[:pInteracts|pInteracts|pInteracts*]->(x3), (x0)-[:pInteracts*]->(x4) RETURN DISTINCT x3, x2, x0, x1;
