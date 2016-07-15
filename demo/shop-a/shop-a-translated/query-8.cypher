MATCH (x0)<-[:pnationality]-()<-[:pcontactPoint]-(x1), (x1)-[:pfaxNumber]->()<-[:pprintPage]-(x2), (x2)-[:pauthor]->()-[:plike]->()<-[:plike]-()-[:page]->(x3) RETURN "true" LIMIT 1;
