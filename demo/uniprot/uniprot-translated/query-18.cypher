MATCH (x0)-[:p0|p0*]->(x1), (x1)-[:p0]->()-[:p1]->()<-[:p1]-()-[:p3]->(x2), (x2)-[:p5]->(x3) RETURN "true" LIMIT 1;
