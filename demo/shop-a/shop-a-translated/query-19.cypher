MATCH (x0)<-[:ptag]-()<-[:plike]-()<-[:pfriendOf]-()-[:plike]->(x1), (x0)<-[:ptag]-()-[:peditor]->()-[:pfamilyName]->()<-[:ptext]-(x2), (x0)<-[:ptag]-()<-[:phasGenre]-(x3) RETURN DISTINCT x0, x1;
