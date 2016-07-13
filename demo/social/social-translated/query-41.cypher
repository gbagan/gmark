MATCH (x0)-[:p21*]->(x1), (x0)-[:p15|p22|p21*]->(x2), (x0)-[:p22|p22|p22*]->(x3), (x0)-[:p21*]->(x4) RETURN DISTINCT x1, x2, x0 UNION  UNION ;
