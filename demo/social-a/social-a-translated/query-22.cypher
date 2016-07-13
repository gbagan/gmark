MATCH (x0)-[:pname]->()<-[:planguage]-()-[:pimageFile]->()<-[:pname]-(x1), (x1)-[:pname|pname*]->(x2), (x2)-[:pisPartOf]->()-[:pname]->()<-[:pcontent]-(x3) RETURN "true" LIMIT 1 UNION  UNION ;
