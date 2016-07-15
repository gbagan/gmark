MATCH (x0)-[:pname|pname*]->(x1), (x1)<-[:pgender]-()<-[:phasMember]-(x2), (x2)-[:pcreationDate|plength|plength*]->(x3) RETURN DISTINCT x0, x3 UNION ;
