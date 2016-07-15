MATCH (x0)-[:p11]->()<-[:p11]-(x1), (x1)-[:p26|p15|p26*]->(x2), (x0)-[:p26|p15|p26*]->(x3), (x3)-[:p26|p26*]->(x2) RETURN "true" LIMIT 1 UNION ;
