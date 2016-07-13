MATCH (x0)-[:planguage*]->(x1), (x0)-[:planguage|planguage*]->(x2), (x1)-[:planguage*]->(x3) RETURN DISTINCT x0;
