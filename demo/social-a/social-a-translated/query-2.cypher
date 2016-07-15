MATCH (x0)-[:pname|pname*]->(x1), (x1)-[:pname*]->(x2), (x2)-[:pname|pname*]->(x3), (x3)-[:pname|pname*]->(x4) RETURN DISTINCT x0, x4;
