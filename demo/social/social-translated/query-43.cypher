MATCH (x0)-[:p23|p24*]->(x1), (x0)-[:p25|p10*]->(x2), (x1)-[:p25|p13|p23*]->(x3) RETURN DISTINCT x3, x0, x1, x2;
