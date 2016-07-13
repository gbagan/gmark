MATCH (x0)-[:pcontent|pbrowserUsed*]->(x1), (x1)-[:pgender|pname|pcontent*]->(x2), (x2)<-[:pgender]-()-[:pspeaks]->()<-[:planguage]-(x3), (x3)-[:pcontent|pcontent*]->(x4) RETURN DISTINCT x0, x4;
