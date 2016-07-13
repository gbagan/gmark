MATCH (x0)<-[:p5]-()<-[:p3]-()-[:p1]->(x1), (x0)<-[:p5]-(x2), (x1)<-[:p5]-(x3) RETURN DISTINCT x0, x1;
