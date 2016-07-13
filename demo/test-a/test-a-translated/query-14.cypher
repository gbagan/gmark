MATCH (x0)<-[:pextendedTo]-()<-[:pauthors]-(x1), (x0)<-[:pextendedTo]-()-[:ppublishedIn]->()-[:pheldIn]->(x2), (x1)<-[:pextendedTo]-()-[:ppublishedIn]->()-[:pheldIn]->(x3) RETURN DISTINCT x0;
