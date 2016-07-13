MATCH (x0)-[:pname*]->(x1), (x0)-[:pname|pname|pname*]->(x2), (x1)-[:pname*]->(x3) RETURN DISTINCT x0, x1;
