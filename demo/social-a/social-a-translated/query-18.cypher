MATCH (x0)-[:pname|pname|pname*]->(x1), (x1)<-[:pworksAt]-()-[:plikes]->()<-[:preplyOf]-(x2), (x2)-[:planguage|pcontent*]->(x3) RETURN "true" LIMIT 1;
