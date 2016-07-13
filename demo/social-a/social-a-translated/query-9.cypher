MATCH (x0)-[:pcontent]->()<-[:pbrowserUsed]-()-[:pname]->()<-[:pname]-(x1), (x1)-[:pname*]->(x2), (x2)<-[:phasCreator]-()<-[:pisSubclassOf]-()-[:pisSubclassOf]->(x3) RETURN DISTINCT x0, x1, x2;
