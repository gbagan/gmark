MATCH (x0)-[:pincludes*]->(x1), (x1)<-[:plike]-()<-[:peditor]-(x2), (x2)-[:pauthor|phasReview|ppurchaseFor*]->(x3), (x3)-[:phasReview*]->(x4) RETURN DISTINCT x3, x0, x2, x1;
