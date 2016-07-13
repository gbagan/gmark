MATCH (x0)-[:pname|pname*]->(x1), (x0)-[:pname|pname*]->(x2), (x0)-[:pname|pname|pname*]->(x3), (x0)-[:pname|pname*]->(x4) RETURN DISTINCT x1, x2, x0 UNION  UNION ;
