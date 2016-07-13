MATCH (x0)-[:pauthor*]->(x1), (x0)-[:phomepage|pfriendOf|peditor*]->(x2), (x0)-[:pactor|pfriendOf*]->(x3) RETURN "true" LIMIT 1;
