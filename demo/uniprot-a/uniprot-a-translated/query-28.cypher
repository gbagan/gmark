MATCH (x0)<-[:pAuthoredBy]-()<-[:pReference]-()-[:pOccursIn]->(x1), (x1)<-[:pOccursIn]-()-[:pOccursIn]->(x2), (x2)<-[:pOccursIn]-()-[:pInteracts]->()-[:pHasKeyword]->(x3) RETURN DISTINCT x0, x3;
