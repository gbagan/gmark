MATCH (x0)-[:ppurchaseFor*]->(x1), (x0)-[:ppurchaseFor*]->(x2), (x0)-[:pmakesPurchase|ppurchaseFor|ppurchaseFor*]->(x3), (x0)-[:ppurchaseFor*]->(x4) RETURN "true" LIMIT 1;
