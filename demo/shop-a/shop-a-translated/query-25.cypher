MATCH (x0)-[:ppurchaseFor*]->(x1), (x0)-[:phomepage|peditor*]->(x2), (x1)-[:pincludes|peditor*]->(x3) RETURN DISTINCT x0, x1;
