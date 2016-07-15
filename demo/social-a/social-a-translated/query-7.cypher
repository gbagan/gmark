MATCH (x0)-[:planguage*]->(x1), (x0)-[:planguage*]->(x2), (x0)-[:planguage|planguage|planguage*]->(x3) RETURN DISTINCT x2, x1, x0, x3;
