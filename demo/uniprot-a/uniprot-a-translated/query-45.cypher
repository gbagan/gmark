MATCH (x0)<-[:pOccursIn]-()-[:pReference]->()-[:pPublishedIn]->(x1), (x0)<-[:pOccursIn]-()-[:pOccursIn]->(x2), (x1)<-[:pOccursIn]-()-[:pOccursIn]->(x3) RETURN DISTINCT x1, x2, x3, x0;
