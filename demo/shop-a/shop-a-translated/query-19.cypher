MATCH (x0)-[:phomepage|phomepage*]->(x1), (x0)-[:ppurchaseFor|phasReview*]->(x2), (x0)-[:phomepage|phomepage*]->(x3), (x0)-[:ppurchaseFor|phomepage|phomepage*]->(x4) RETURN DISTINCT x0, x2, x1, x3;
