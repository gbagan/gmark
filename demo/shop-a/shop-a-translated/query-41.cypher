MATCH (x0)<-[:ptag]-()<-[:plike]-()-[:page]->(x1), (x0)<-[:ptag]-()<-[:plike]-()-[:ptype]->(x2), (x1)<-[:ptag]-()<-[:plike]-()-[:ptype]->(x3) RETURN DISTINCT x0, x1, x2;
