MATCH (x0)<-[:pheldIn]-()-[:pheldIn]->()<-[:pheldIn]-()<-[:ppublishedIn]-(x1), (x1)-[:ppublishedIn]->()<-[:ppublishedIn]-(x2), (x2)-[:ppublishedIn*]->(x3), (x3)-[:ppublishedIn*]->(x4) RETURN DISTINCT x0;