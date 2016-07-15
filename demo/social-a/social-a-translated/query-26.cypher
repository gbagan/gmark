MATCH (x0)-[:pname|pname*]->(x1), (x0)-[:pstudyAt|pname*]->(x2), (x1)-[:pstudyAt*]->(x3) RETURN "true" LIMIT 1;
