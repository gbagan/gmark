MATCH (x0)-[:pname]->()<-[:pgender]-(x1), (x1)-[:pcreationDate|plocationIP*]->(x2), (x0)-[:pname]->()<-[:pname]-(x3), (x2)-[:pname]->()<-[:pname]-(x4) RETURN DISTINCT x0, x2;
