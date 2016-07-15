MATCH (x0)-[:pname|pname|pname*]->(x1), (x1)-[:pname|pname|pname*]->(x2), (x0)-[:pname|pname*]->(x3), (x3)-[:pname*]->(x2) RETURN DISTINCT x0;
