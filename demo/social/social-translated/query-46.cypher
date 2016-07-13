MATCH (x0)<-[:p17]-()-[:p20]->()<-[:p26]-(x1), (x1)-[:p26|p26|p15*]->(x2), (x2)-[:p26|p26|p26*]->(x3) RETURN DISTINCT x0, x3;
