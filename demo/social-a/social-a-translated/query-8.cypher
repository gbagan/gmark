MATCH (x0)-[:preplyOf]->()<-[:plikes]-()-[:plocationIP]->()<-[:pname]-(x1), (x1)-[:pname*]->(x2), (x2)-[:pname]->()<-[:pbrowserUsed]-()-[:pemail]->()<-[:pname]-(x3) RETURN DISTINCT x0;
