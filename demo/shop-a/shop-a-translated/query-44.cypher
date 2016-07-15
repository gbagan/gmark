MATCH (x0)<-[:ptype]-()-[:ptag]->()<-[:ptag]-(x1), (x0)<-[:ptype]-()<-[:phasGenre]-(x2), (x1)<-[:ptype]-()-[:ptag]->()<-[:ptag]-(x3) RETURN "true" LIMIT 1;
