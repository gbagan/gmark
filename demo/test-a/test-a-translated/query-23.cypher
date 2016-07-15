MATCH (x0)<-[:pheldIn]-()<-[:ppublishedIn]-()-[:ppublishedIn]->(x1), (x1)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x2), (x2)-[:pheldIn]->()<-[:pheldIn]-()-[:pheldIn]->(x3) RETURN DISTINCT x0;
