MATCH (x0)-[:pname|pname*]->(x1), (x0)-[:pname*]->(x2), (x0)-[:pname|pname*]->(x3) RETURN DISTINCT x0, x2, x1 UNION ;
