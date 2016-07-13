MATCH (x0)<-[:pextendedTo]-()-[:ppublishedIn]->(x1), (x0)<-[:pextendedTo]-()<-[:pauthors]-(x2), (x1)<-[:pextendedTo]-()<-[:pauthors]-(x3) RETURN "true" LIMIT 1;
