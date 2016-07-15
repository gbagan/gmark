MATCH (x0)-[:pInteracts|pInteracts*]->(x1), (x0)-[:pInteracts|pInteracts|pInteracts*]->(x2), (x2)-[:pInteracts|pInteracts*]->(x1) RETURN DISTINCT x0, x2, x1;
