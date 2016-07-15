MATCH (x0)-[:phomepage|phomepage*]->(x1), (x1)-[:psubscribes*]->(x2), (x2)<-[:phomepage]-()-[:phomepage]->()<-[:phomepage]-(x3), (x3)-[:phomepage|pfriendOf|pauthor*]->(x4) RETURN "true" LIMIT 1;
