MATCH (x0)-[:pconductor*]->(x1), (x1)<-[:partist]-()<-[:ppurchaseFor]-(x2), (x2)-[:ppurchaseFor|ppurchaseFor*]->(x3) RETURN DISTINCT x0, x3;
