MATCH (x0)-[:p15|p15*]->(x1), (x1)-[:p26|p26*]->(x2), (x0)-[:p15*]->(x3), (x3)-[:p3]->()<-[:p0]-()-[:p6]->(x2) RETURN DISTINCT x0 UNION ;
