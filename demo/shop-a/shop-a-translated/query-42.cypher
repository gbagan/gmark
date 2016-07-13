MATCH (x0)<-[:pmakesPurchase]-()-[:pfollows]->()<-[:pfollows]-()<-[:pconductor]-(x1), (x1)<-[:ppurchaseFor]-()-[:ppurchaseFor]->(x2), (x2)-[:pincludes*]->(x3) RETURN DISTINCT x0, x3;
