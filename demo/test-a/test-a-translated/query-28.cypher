MATCH (x0)<-[:pheldIn]-()<-[:ppublishedIn]-()-[:pextendedTo]->(x1), (x1)<-[:pextendedTo]-()-[:ppublishedIn]->()<-[:ppublishedIn]-()<-[:pauthors]-(x2), (x2)-[:pauthors]->()-[:pextendedTo]->()<-[:pextendedTo]-(x3), (x3)<-[:pauthors]-()-[:pauthors]->()<-[:pauthors]-(x4) RETURN DISTINCT x2, x0, x1;