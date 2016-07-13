MATCH (x0)-[:pauthor]->()-[:plike]->()<-[:plike]-()<-[:pauthor]-(x1), (x1)<-[:plike]-()<-[:pauthor]-()<-[:pincludes]-(x2), (x2)-[:pincludes|pincludes*]->(x3) RETURN "true" LIMIT 1;
