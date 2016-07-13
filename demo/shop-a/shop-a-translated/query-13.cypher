MATCH (x0)-[:pmakesPurchase*]->(x1), (x0)-[:ppurchaseFor*]->(x2), (x0)-[:ppurchaseFor|ppurchaseFor*]->(x3) RETURN DISTINCT x0, x1;
