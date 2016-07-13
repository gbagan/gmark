MATCH (x0)-[:pdirector|phomepage*]->(x1), (x1)-[:pincludes*]->(x2), (x2)-[:pdirector]->()-[:plike]->()<-[:plike]-()<-[:previewer]-(x3), (x3)-[:ptitle]->()<-[:ptitle]-(x4) RETURN DISTINCT x0, x4;
