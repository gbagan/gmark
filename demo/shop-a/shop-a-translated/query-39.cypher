MATCH (x0)<-[:pnumberOfPages]-()<-[:ppurchaseFor]-(x1), (x1)-[:ppurchaseFor|ppurchaseFor|ppurchaseFor*]->(x2), (x2)-[:ppurchaseFor*]->(x3) RETURN DISTINCT x1, x0, x2, x3;
