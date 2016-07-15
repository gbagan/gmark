MATCH (x0)-[:phasReview*]->(x1), (x0)-[:peditor*]->(x2), (x0)-[:phomepage|pauthor|peditor*]->(x3) RETURN DISTINCT x0, x2, x1;
