MATCH (x0)-[:pgender|pbrowserUsed|pcontent*]->(x1), (x0)-[:pbrowserUsed|pbrowserUsed*]->(x2), (x1)-[:pgender|pname|pname*]->(x3) RETURN "true" LIMIT 1;
