MATCH (x0)-[:p3]->()-[:p5]->(x1), (x1)<-[:p5]-()-[:p5]->()<-[:p5]-()<-[:p3]-(x2), (x2)-[:p0]->()<-[:p0]-()-[:p3]->(x3), (x3)<-[:p3]-()-[:p3]->()-[:p6]->()<-[:p6]-(x4) RETURN DISTINCT x0, x1, x2;