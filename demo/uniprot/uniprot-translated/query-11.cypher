MATCH (x0)-[:p0|p0|p0*]->(x1), (x0)-[:p0|p0|p0*]->(x2), (x2)-[:p0*]->(x1) RETURN DISTINCT x0, x1;
