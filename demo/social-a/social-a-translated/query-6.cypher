MATCH (x0)-[:pname|pname|pname*]->(x1), (x1)-[:pname]->()<-[:pspeaks]-()-[:plocationIP]->()<-[:pname]-(x2), (x2)-[:pname*]->(x3) RETURN DISTINCT x0, x3 UNION  UNION ;
