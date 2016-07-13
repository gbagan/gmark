MATCH (x0)-[:phomepage|psubscribes|ptrailer*]->(x1), (x0)-[:phomepage|ptrailer*]->(x2), (x0)-[:phomepage|phomepage*]->(x3) RETURN DISTINCT x0, x1;
