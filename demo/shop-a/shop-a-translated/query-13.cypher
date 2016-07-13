MATCH (x0)-[:pauthor]->()-[:plike]->()<-[:plike]-()<-[:pcontactPoint]-(x1), (x1)-[:pemployee*]->(x2), (x2)-[:pcontactPoint]->()-[:plike]->()<-[:plike]-(x3) RETURN DISTINCT x0;
