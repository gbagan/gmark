MATCH (x0)-[:phomepage|pactor|phomepage*]->(x1), (x1)-[:phomepage|pactor|ptrailer*]->(x2), (x0)-[:pincludes|pdirector|pdirector*]->(x3), (x2)-[:pincludes*]->(x4) RETURN DISTINCT x1, x2, x3, x0;
