MATCH (x0)-[:pincludes]->()-[:pcontentRating]->()<-[:pprintSection]-(x1), (x0)-[:pincludes*]->(x2), (x2)-[:pincludes|pincludes|pincludes*]->(x1) RETURN DISTINCT x0;
