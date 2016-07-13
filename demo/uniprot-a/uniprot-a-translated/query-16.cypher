MATCH (x0)-[:pHasKeyword]->()<-[:pHasKeyword]-()-[:pInteracts]->(x1), (x0)-[:pReference]->()<-[:pReference]-(x2), (x0)-[:pOccursIn]->()<-[:pOccursIn]-()-[:pInteracts]->(x3) RETURN "true" LIMIT 1;
