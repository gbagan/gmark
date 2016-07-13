MATCH (x0)-[:pincludes|pincludes|pincludes*]->(x1), (x1)-[:pincludes|pincludes*]->(x2), (x0)-[:pincludes*]->(x3), (x2)-[:pincludes|pincludes|pincludes*]->(x4) RETURN DISTINCT x1, x3, x0, x2;
