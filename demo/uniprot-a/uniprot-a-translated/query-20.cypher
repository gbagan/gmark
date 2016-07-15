MATCH (x0)<-[:pReference]-()<-[:pInteracts]-()-[:pInteracts]->(x1), (x1)-[:pInteracts|pInteracts|pInteracts*]->(x2), (x0)-[:pAuthoredBy]->(x3), (x2)-[:pAuthoredBy]->(x4) RETURN DISTINCT x0;
