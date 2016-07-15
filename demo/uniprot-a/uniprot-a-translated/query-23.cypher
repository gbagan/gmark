MATCH (x0)-[:pInteracts|pInteracts|pInteracts*]->(x1), (x0)-[:pInteracts|pInteracts*]->(x2), (x1)-[:pInteracts*]->(x3) RETURN "true" LIMIT 1;
