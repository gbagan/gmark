MATCH (x0)-[:planguage|pcontent*]->(x1), (x1)-[:pisSubclassOf]->()<-[:plikes]-()-[:pcreationDate]->(x2), (x0)-[:planguage*]->(x3), (x2)-[:planguage*]->(x4) RETURN DISTINCT x2, x3, x0, x1 UNION ;
