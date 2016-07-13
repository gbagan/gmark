MATCH (x0)-[:p0*]->(x1), (x1)-[:p0|p0|p0*]->(x2), (x2)-[:p2]->()<-[:p2]-()-[:p3]->(x3) RETURN DISTINCT x0, x3;
