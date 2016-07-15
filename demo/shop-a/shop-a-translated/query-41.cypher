MATCH (x0)-[:phomepage*]->(x1), (x0)-[:phasReview|phasReview*]->(x2), (x0)-[:peditor|peditor|phomepage*]->(x3), (x0)-[:peditor|phasReview|phomepage*]->(x4) RETURN "true" LIMIT 1;
