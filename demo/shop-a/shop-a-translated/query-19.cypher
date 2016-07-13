MATCH (x0)<-[:pemail]-()<-[:pauthor]-(x1), (x1)-[:pincludes|pauthor|ppurchaseFor*]->(x2), (x2)-[:pincludes|pincludes|phomepage*]->(x3) RETURN DISTINCT x0, x3;
