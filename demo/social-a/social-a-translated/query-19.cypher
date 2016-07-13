MATCH (x0)-[:pcontent]->()<-[:pgender]-()-[:pbirthday]->(x1), (x1)-[:pbirthday*]->(x2), (x2)<-[:pcreationDate]-()-[:pgender]->()<-[:pname]-()<-[:phasTag]-(x3) RETURN DISTINCT x0, x3;
