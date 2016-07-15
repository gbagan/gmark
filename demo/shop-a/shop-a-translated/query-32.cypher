MATCH (x0)-[:ptrailer|psubscribes|psubscribes*]->(x1), (x1)<-[:phomepage]-()<-[:partist]-()-[:phasReview]->(x2), (x2)-[:phasReview*]->(x3) RETURN "true" LIMIT 1;
