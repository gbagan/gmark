MATCH (x0)-[:p21|p23|p21*]->(x1), (x0)-[:p25|p25*]->(x2), (x0)-[:p16|p23*]->(x3), (x0)-[:p22*]->(x4) RETURN DISTINCT x0, x1;
