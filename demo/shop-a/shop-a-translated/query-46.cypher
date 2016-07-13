MATCH (x0)-[:ppurchaseFor|ppurchaseFor|phasReview*]->(x1), (x0)-[:phasReview|phasReview*]->(x2), (x0)-[:phasReview|phasReview|phomepage*]->(x3), (x0)-[:phomepage*]->(x4) RETURN DISTINCT x0, x1;
