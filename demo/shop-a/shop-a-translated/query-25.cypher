MATCH (x0)-[:peditor|pauthor*]->(x1), (x1)<-[:ppurchaseFor]-()-[:ppurchaseFor]->()-[:phomepage]->()<-[:phomepage]-(x2), (x2)-[:phasReview|phomepage|phasReview*]->(x3) RETURN DISTINCT x0, x3;
