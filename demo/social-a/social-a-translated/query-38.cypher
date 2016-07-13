MATCH (x0)-[:pgender|pgender*]->(x1), (x0)-[:pcontent|pname*]->(x2), (x1)-[:pname|pbrowserUsed*]->(x3) RETURN DISTINCT x0, x1;
