MATCH (x0)-[:pincludes*]->(x1), (x1)-[:pvalidThrough]->()<-[:prelease]-(x2), (x0)-[:pincludes|pincludes*]->(x3), (x3)-[:pincludes|pincludes*]->(x2) RETURN DISTINCT x3, x2, x0, x1;
