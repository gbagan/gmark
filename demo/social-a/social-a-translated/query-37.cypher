MATCH (x0)-[:pname|planguage*]->(x1), (x0)-[:plength*]->(x2), (x1)-[:pemail|pspeaks*]->(x3) RETURN DISTINCT x0, x1;
