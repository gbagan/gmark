MATCH (x0)-[:pincludes*]->(x1), (x1)-[:pincludes|pincludes*]->(x2), (x2)-[:pincludes]->()<-[:plike]-()<-[:partist]-(x3), (x3)-[:partist]->()-[:phomepage]->(x4) RETURN DISTINCT x0, x4;
