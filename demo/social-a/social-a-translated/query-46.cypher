MATCH (x0)-[:pimageFile|pgender*]->(x1), (x1)-[:pbrowserUsed|pcontent|pbrowserUsed*]->(x2), (x0)-[:pname*]->(x3), (x2)-[:pgender|pname|plocationIP*]->(x4) RETURN "true" LIMIT 1;
