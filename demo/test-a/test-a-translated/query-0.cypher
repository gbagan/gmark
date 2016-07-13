MATCH (x0)<-[:pheldIn]-()<-[:ppublishedIn]-()-[:ppublishedIn]->(x1), (x0)<-[:pheldIn]-()<-[:ppublishedIn]-(x2), (x1)<-[:pheldIn]-()<-[:ppublishedIn]-(x3) RETURN DISTINCT x0, x1;
