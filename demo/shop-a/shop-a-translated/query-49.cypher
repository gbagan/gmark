MATCH (x0)-[:precordNumber]->()<-[:pprice]-()-[:pincludes]->(x1), (x1)<-[:plike]-()<-[:pdirector]-(x2), (x2)-[:phasReview*]->(x3) RETURN DISTINCT x3, x1, x2, x0;
