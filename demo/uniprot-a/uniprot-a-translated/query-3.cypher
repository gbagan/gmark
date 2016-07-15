MATCH (x0)<-[:pAuthoredBy]-()-[:pPublishedIn]->(x1), (x0)<-[:pAuthoredBy]-()<-[:pReference]-()<-[:pInteracts]-(x2), (x2)<-[:pInteracts]-()-[:pReference]->(x1) RETURN DISTINCT x0, x2, x1;
