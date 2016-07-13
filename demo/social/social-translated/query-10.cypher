MATCH (x0)-[:p16|p25*]->(x1), (x1)-[:p22*]->(x2), (x0)-[:p19*]->(x3), (x2)-[:p23*]->(x4) RETURN DISTINCT x2, x1, x0, x3 UNION ;
