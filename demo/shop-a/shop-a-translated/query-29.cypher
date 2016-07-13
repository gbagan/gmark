MATCH (x0)<-[:ptype]-()-[:plike]->(x1), (x1)-[:pdirector]->()<-[:pfriendOf]-()<-[:peditor]-(x2), (x2)-[:pauthor]->()-[:plocation]->(x3) RETURN DISTINCT x0;
