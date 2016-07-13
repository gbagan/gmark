MATCH (x0)<-[:pOccursIn]-()-[:pReference]->()-[:pPublishedIn]->(x1), (x1)<-[:pPublishedIn]-()<-[:pReference]-()<-[:pInteracts]-(x2), (x2)-[:pInteracts*]->(x3) RETURN DISTINCT x3, x0, x1, x2;
