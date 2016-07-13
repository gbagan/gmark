MATCH (x0)-[:pcontent*]->(x1), (x0)-[:pcontent|planguage|planguage*]->(x2), (x0)-[:pimageFile*]->(x3) RETURN "true" LIMIT 1;
