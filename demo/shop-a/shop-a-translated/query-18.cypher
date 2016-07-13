MATCH (x0)-[:pmakesPurchase|ppurchaseFor|ppurchaseFor*]->(x1), (x0)-[:ppurchaseFor*]->(x2), (x0)-[:pmakesPurchase|pmakesPurchase|ppurchaseFor*]->(x3) RETURN DISTINCT x2, x1, x3, x0;
