MATCH (x0)-[:pisLocatedIn]->()-[:pname]->()<-[:plength]-(x1), (x1)-[:pcreationDate|plength|plength*]->(x2), (x2)-[:plength]->()<-[:pcontent]-()-[:planguage]->(x3) RETURN "true" LIMIT 1;
