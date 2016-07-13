MATCH (x0)-[:p15*]->(x1), (x1)-[:p2]->()<-[:p9]-()<-[:p8]-()-[:p18]->(x2), (x0)-[:p15|p15*]->(x3), (x2)-[:p26|p26|p15*]->(x4) RETURN DISTINCT x0, x2 UNION  UNION  UNION  UNION ;
