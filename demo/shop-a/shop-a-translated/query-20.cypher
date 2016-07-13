MATCH (x0)-[:phasReview]->()-[:previewer]->()-[:plike]->()<-[:plike]-(x1), (x1)-[:phomepage|pfriendOf|previewer*]->(x2), (x2)-[:pmakesPurchase]->()-[:ppurchaseFor]->(x3) RETURN DISTINCT x0, x2, x1;
