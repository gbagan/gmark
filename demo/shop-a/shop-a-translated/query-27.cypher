MATCH (x0)-[:ppurchaseFor*]->(x1), (x1)-[:phomepage|pconductor*]->(x2), (x2)-[:pcomposer]->()<-[:pcaption]-()<-[:plike]-()-[:page]->(x3) RETURN DISTINCT x0, x3;
