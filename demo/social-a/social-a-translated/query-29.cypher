MATCH (x0)-[:pcontent*]->(x1), (x0)-[:pcontent*]->(x2), (x1)-[:pcontent|pcontent*]->(x3) RETURN "true" LIMIT 1 UNION ;
