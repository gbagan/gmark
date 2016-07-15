MATCH (x0)-[:p23|p24|p24*]->(x1), (x1)-[:p23|p23|p24*]->(x2), (x0)-[:p13|p14|p13*]->(x3), (x2)-[:p13|p13*]->(x4) RETURN DISTINCT x1, x0, x2;
