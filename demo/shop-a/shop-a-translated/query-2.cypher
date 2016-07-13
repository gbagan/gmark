MATCH (x0)-[:pincludes|pauthor|ppurchaseFor*]->(x1), (x0)-[:phasReview|phasReview*]->(x2), (x1)-[:peditor|peditor|phomepage*]->(x3) RETURN DISTINCT x2, x1, x0;
