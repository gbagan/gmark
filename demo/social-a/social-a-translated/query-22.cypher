MATCH (x0)-[:pname*]->(x1), (x0)-[:pname|pname*]->(x2), (x0)-[:pname*]->(x3), (x0)-[:pname|pname|pname*]->(x4) RETURN DISTINCT x1, x3, x2, x0 UNION ;
