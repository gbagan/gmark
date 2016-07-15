MATCH (x0)-[:pname|pname*]->(x1), (x1)-[:pname|pname*]->(x2), (x0)-[:pname]->()<-[:pcontent]-()-[:planguage]->(x3), (x3)-[:planguage*]->(x2) RETURN DISTINCT x0, x2;
