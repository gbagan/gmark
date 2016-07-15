MATCH (x0)-[:ppurchaseFor*]->(x1), (x1)-[:phomepage]->()<-[:phomepage]-()<-[:plike]-()<-[:pauthor]-(x2), (x2)-[:phomepage|pincludes*]->(x3) RETURN "true" LIMIT 1;
