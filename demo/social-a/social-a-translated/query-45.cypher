MATCH (x0)-[:plocationIP|plocationIP|pbirthday*]->(x1), (x1)-[:pbrowserUsed*]->(x2), (x2)-[:pbrowserUsed|pcreationDate*]->(x3) RETURN DISTINCT x0, x3;
