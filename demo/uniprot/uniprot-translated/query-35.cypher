MATCH (x0)<-[:p4]-()-[:p1]->(x1), (x1)<-[:p1]-()-[:p2]->()<-[:p2]-(x2), (x2)-[:p0|p0|p0*]->(x3) RETURN DISTINCT x2, x0, x1;
