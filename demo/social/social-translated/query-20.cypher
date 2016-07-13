MATCH (x0)-[:p15|p17*]->(x1), (x1)-[:p22|p16*]->(x2), (x2)-[:p22|p16*]->(x3), (x3)-[:p4]->()<-[:p4]-()-[:p21]->(x4) RETURN DISTINCT x0, x4 UNION  UNION ;
