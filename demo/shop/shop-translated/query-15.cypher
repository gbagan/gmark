MATCH (x0)-[:p74*]->(x1), (x1)-[:p75|p74|p75*]->(x2), (x2)-[:p74|p75|p74*]->(x3) RETURN DISTINCT x0;
