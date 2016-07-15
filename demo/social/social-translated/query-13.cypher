MATCH (x0)-[:p16|p16*]->(x1), (x1)-[:p16|p16*]->(x2), (x0)-[:p5|p16*]->(x3), (x2)-[:p5|p5|p5*]->(x4) RETURN DISTINCT x0, x1, x2, x3;
