MATCH (x0)<-[:ptag]-()-[:pcaption]->(x1), (x0)<-[:ptag]-()<-[:phasGenre]-()-[:pproducer]->(x2), (x0)<-[:ptag]-()-[:pkeywords]->()<-[:pcaption]-()-[:ptitle]->(x3) RETURN DISTINCT x0, x1;
