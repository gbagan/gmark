MATCH (x0)-[:pname]->()<-[:pname]-(x1), (x0)-[:pname|pname*]->(x2), (x2)-[:pname|pname*]->(x1) RETURN "true" LIMIT 1;
