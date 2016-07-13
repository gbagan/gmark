MATCH (x0)<-[:ptype]-()<-[:pactor]-()-[:ptag]->(x1), (x0)<-[:ptype]-()<-[:peditor]-()-[:ptag]->(x2), (x0)<-[:ptype]-()-[:plike]->()-[:ptag]->(x3) RETURN DISTINCT x1, x2, x3, x0;
