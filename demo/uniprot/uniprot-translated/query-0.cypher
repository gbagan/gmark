MATCH (x0)<-[:p4]-()-[:p1]->(x1), (x1)<-[:p1]-()<-[:p0]-()-[:p3]->(x2), (x2)-[:p5]->(x3) RETURN DISTINCT x0;
