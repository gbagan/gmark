MATCH (x0)<-[:planguage]-()-[:pcontent]->()<-[:plocationIP]-()<-[:phasCreator]-(x1), (x1)-[:plocationIP*]->(x2), (x2)-[:pbrowserUsed*]->(x3) RETURN DISTINCT x0, x3 UNION ;
