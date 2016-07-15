MATCH (x0)<-[:pAuthoredBy]-()<-[:pReference]-()-[:pReference]->(x1), (x0)<-[:pAuthoredBy]-()<-[:pReference]-()-[:pInteracts]->(x2), (x2)-[:pInteracts*]->(x1) RETURN DISTINCT x1, x2, x0;
