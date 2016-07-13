MATCH (x0)-[:p22*]->(x1), (x0)-[:p22|p22|p22*]->(x2), (x0)-[:p15|p22*]->(x3) RETURN DISTINCT x0, x1, x3, x2 UNION ;
