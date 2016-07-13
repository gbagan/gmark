MATCH (x0)-[:pincludes*]->(x1), (x0)-[:pprice|pincludes*]->(x2), (x1)-[:peditor|pprice*]->(x3) RETURN DISTINCT x3, x1, x2, x0;
