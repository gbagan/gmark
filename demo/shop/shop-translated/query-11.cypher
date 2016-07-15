MATCH (x0)-[:p80|p81|p81*]->(x1), (x0)-[:p81*]->(x2), (x1)-[:p80*]->(x3) RETURN DISTINCT x0, x2, x3, x1;
