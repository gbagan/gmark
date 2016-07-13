MATCH (x0)-[:pcreationDate*]->(x1), (x0)-[:pcreationDate|pcreationDate|pcreationDate*]->(x2), (x0)-[:pcreationDate|pcreationDate*]->(x3) RETURN DISTINCT x0;
