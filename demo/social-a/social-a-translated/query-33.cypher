MATCH (x0)-[:pname*]->(x1), (x1)-[:pname|pname*]->(x2), (x0)-[:pname*]->(x3), (x3)-[:pname]->()<-[:pbrowserUsed]-()-[:pstudyAt]->(x2) RETURN DISTINCT x2, x1, x0;
