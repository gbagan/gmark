MATCH (x0)<-[:pkeywords]-()<-[:plike]-()<-[:partist]-()-[:ptext]->(x1), (x0)<-[:ptext]-()-[:phasGenre]->()-[:ptype]->(x2), (x1)<-[:ptext]-()-[:phasGenre]->()-[:ptype]->(x3) RETURN DISTINCT x0;
