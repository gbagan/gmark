MATCH (x0)<-[:psubscribes]-()-[:plike]->()<-[:plike]-()<-[:pauthor]-(x1), (x1)<-[:plike]-()<-[:pconductor]-(x2), (x2)-[:ppurchaseFor*]->(x3) RETURN DISTINCT x0, x3;
