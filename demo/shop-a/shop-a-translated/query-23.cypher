MATCH (x0)-[:pauthor|pauthor|peditor*]->(x1), (x0)-[:phasReview|pincludes|ppurchaseFor*]->(x2), (x1)-[:pauthor|ppurchaseFor|phasReview*]->(x3) RETURN DISTINCT x0;
