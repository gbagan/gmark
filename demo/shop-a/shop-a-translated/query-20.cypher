MATCH (x0)-[:phomepage|phomepage|pactor*]->(x1), (x0)-[:pdirector|ptrailer|pdirector*]->(x2), (x2)-[:phasReview|ptrailer|pactor*]->(x1) RETURN DISTINCT x0, x2, x1;
