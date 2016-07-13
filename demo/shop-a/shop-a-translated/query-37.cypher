MATCH (x0)-[:phasReview|ppurchaseFor*]->(x1), (x0)-[:ppurchaseFor*]->(x2), (x1)-[:ppurchaseFor|phomepage|phomepage*]->(x3) RETURN DISTINCT x2, x1, x0, x3;
