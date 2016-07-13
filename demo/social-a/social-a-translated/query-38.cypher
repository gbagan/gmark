MATCH (x0)-[:pcreationDate*]->(x1), (x0)-[:plength|plength*]->(x2), (x0)-[:plength|plength|plength*]->(x3) RETURN DISTINCT x0, x1;
