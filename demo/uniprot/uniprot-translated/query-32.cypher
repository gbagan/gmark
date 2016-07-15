MATCH (x0)<-[:p1]-()-[:p0]->()-[:p0]->()<-[:p0]-(x1), (x1)-[:p0*]->(x2), (x2)-[:p0*]->(x3) RETURN DISTINCT x3, x2, x1, x0;
