MATCH (x0)-[:phasReview|phasReview|phomepage*]->(x1), (x1)-[:pdirector*]->(x2), (x2)-[:pactor|ptrailer|phomepage*]->(x3) RETURN DISTINCT x2, x1, x0;
