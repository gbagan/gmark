MATCH (x0)-[:p16|p16|p16*]->(x1), (x1)-[:p16|p16*]->(x2), (x2)<-[:p12]-()<-[:p1]-()-[:p22]->(x3) RETURN DISTINCT x0, x3 UNION ;
