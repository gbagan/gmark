MATCH (x0)-[:pname*]->(x1), (x1)-[:pname]->()<-[:pname]-()-[:pgender]->()<-[:pname]-(x2), (x0)-[:pname|pname|pname*]->(x3), (x2)-[:pname*]->(x4) RETURN DISTINCT x0 UNION ;
