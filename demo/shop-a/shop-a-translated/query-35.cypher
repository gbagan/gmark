MATCH (x0)<-[:ptype]-()-[:ptag]->(x1), (x0)<-[:ptype]-()<-[:phasGenre]-()-[:pperformedIn]->(x2), (x1)<-[:ptype]-()<-[:phasGenre]-()-[:pperformedIn]->(x3) RETURN DISTINCT x3, x0, x1, x2;
