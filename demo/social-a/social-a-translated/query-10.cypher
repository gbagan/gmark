MATCH (x0)-[:pname]->()<-[:pname]-()-[:plocationIP]->()<-[:planguage]-(x1), (x0)-[:pname]->()<-[:pimageFile]-(x2), (x0)-[:pname]->()<-[:pimageFile]-(x3) RETURN "true" LIMIT 1 UNION  UNION ;
