MATCH (x0)-[:p16*]->(x1), (x1)-[:p17]->()<-[:p16]-()-[:p16]->()<-[:p26]-(x2), (x0)-[:p21|p22|p16*]->(x3), (x2)-[:p21|p15*]->(x4) RETURN DISTINCT x0, x2 UNION  UNION ;
