MATCH (x0)-[:pdirector*]->(x1), (x0)-[:ptrailer|pactor*]->(x2), (x0)-[:pdirector|phomepage|pactor*]->(x3) RETURN DISTINCT x0;
