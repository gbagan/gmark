MATCH (x0)<-[:pOccursIn]-()<-[:pInteracts]-(x1), (x0)<-[:pOccursIn]-()-[:pReference]->(x2), (x1)<-[:pOccursIn]-()-[:pHasKeyword]->()<-[:pHasKeyword]-()-[:pReference]->(x3) RETURN "true" LIMIT 1;
