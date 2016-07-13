MATCH (x0)-[:pcreationDate*]->(x1), (x0)-[:pbirthday*]->(x2), (x1)-[:pcreationDate*]->(x3) RETURN DISTINCT x0, x1, x2;
