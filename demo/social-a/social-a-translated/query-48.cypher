MATCH (x0)-[:pgender|pname|pgender*]->(x1), (x0)-[:pgender|pcreationDate|plocationIP*]->(x2), (x1)-[:pbirthday|pname*]->(x3) RETURN "true" LIMIT 1;
