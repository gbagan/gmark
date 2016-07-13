MATCH (x0)-[:plocationIP*]->(x1), (x0)-[:pgender|pname|plocationIP*]->(x2), (x0)-[:pgender|pname|pgender*]->(x3), (x0)-[:pcreationDate|pcreationDate|pgender*]->(x4) RETURN "true" LIMIT 1;
