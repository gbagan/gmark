MATCH (x0)-[:p26|p26*]->(x1), (x0)-[:p26|p26|p26*]->(x2), (x1)-[:p26|p26*]->(x3) RETURN DISTINCT x3, x1, x2, x0;
