MATCH (x0)-[:pgender|pname|pbrowserUsed*]->(x1), (x0)-[:plocationIP*]->(x2), (x1)-[:pname|pbrowserUsed|pname*]->(x3) RETURN DISTINCT x0, x1;
