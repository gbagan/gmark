MATCH (x0)-[:p15*]->(x1), (x0)<-[:p15]-()-[:p20]->()<-[:p20]-(x2), (x2)-[:p19]->()<-[:p21]-(x1) RETURN DISTINCT x1, x0, x2 UNION ;
