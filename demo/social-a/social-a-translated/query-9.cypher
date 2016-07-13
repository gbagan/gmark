MATCH (x0)-[:pname|pname|pname*]->(x1), (x0)-[:pname*]->(x2), (x0)-[:pname|pname|pname*]->(x3), (x0)-[:pname|pname|pname*]->(x4) RETURN DISTINCT x0, x1;
