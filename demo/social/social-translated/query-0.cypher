MATCH (x0)-[:p17|p22*]->(x1), (x0)<-[:p2]-()-[:p2]->()<-[:p3]-(x2), (x2)-[:p26|p26*]->(x1) RETURN DISTINCT x0;
