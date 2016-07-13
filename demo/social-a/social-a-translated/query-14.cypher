MATCH (x0)-[:planguage|pcontent|pcontent*]->(x1), (x1)-[:pcontent*]->(x2), (x0)-[:planguage|pcontent*]->(x3), (x2)-[:pcontent|pcontent*]->(x4) RETURN DISTINCT x0;
