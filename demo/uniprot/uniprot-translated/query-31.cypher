MATCH (x0)<-[:p4]-()-[:p3]->(x1), (x1)-[:p6]->()<-[:p6]-()<-[:p3]-(x2), (x2)-[:p0|p0|p0*]->(x3) RETURN "true" LIMIT 1;
