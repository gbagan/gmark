MATCH (x0)-[:p0|p0|p0*]->(x1), (x0)-[:p0*]->(x2), (x0)-[:p0|p0|p0*]->(x3), (x0)-[:p0*]->(x4) RETURN DISTINCT x3, x1, x0, x2;
