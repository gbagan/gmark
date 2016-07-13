MATCH (x0)-[:pcontent*]->(x1), (x1)-[:pcontent*]->(x2), (x2)-[:pcontent*]->(x3), (x3)-[:pcontent|pcontent|pcontent*]->(x4) RETURN DISTINCT x0, x4 UNION  UNION ;
