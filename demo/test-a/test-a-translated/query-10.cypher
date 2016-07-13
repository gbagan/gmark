MATCH (x0)<-[:pheldIn]-()<-[:ppublishedIn]-()-[:ppublishedIn]->(x1), (x1)-[:ppublishedIn*]->(x2), (x2)-[:ppublishedIn*]->(x3), (x3)-[:ppublishedIn|ppublishedIn*]->(x4) RETURN DISTINCT x0, x1, x2;
