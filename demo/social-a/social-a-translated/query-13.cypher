MATCH (x0)-[:planguage]->()<-[:pbrowserUsed]-()-[:pgender]->()<-[:pname]-(x1), (x1)-[:pname*]->(x2), (x2)-[:pname|pname*]->(x3) RETURN DISTINCT x2, x0, x1;
