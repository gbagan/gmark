MATCH (x0)-[:pincludes|pincludes|pincludes*]->(x1), (x0)-[:pincludes*]->(x2), (x0)-[:pincludes|pincludes|pincludes*]->(x3), (x0)-[:pincludes|pincludes*]->(x4) RETURN DISTINCT x2, x0, x1, x3;
