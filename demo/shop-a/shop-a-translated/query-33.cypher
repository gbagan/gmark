MATCH (x0)-[:phasReview*]->(x1), (x1)-[:peditor*]->(x2), (x2)-[:pprintColumn]->()<-[:phits]-(x3) RETURN DISTINCT x3, x1, x2, x0;
