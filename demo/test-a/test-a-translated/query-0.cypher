MATCH (x0)-[:ppublishedIn*]->(x1), (x1)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x2), (x2)<-[:ppublishedIn]-()-[:pextendedTo]->()<-[:pextendedTo]-()<-[:pauthors]-(x3) RETURN DISTINCT x2, x0, x1;
