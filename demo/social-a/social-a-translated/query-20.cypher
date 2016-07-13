MATCH (x0)-[:pcreationDate|plocationIP*]->(x1), (x0)-[:pcreationDate|pbrowserUsed*]->(x2), (x1)-[:pbrowserUsed*]->(x3) RETURN DISTINCT x1, x2, x0;
