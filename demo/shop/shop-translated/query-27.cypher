MATCH (x0)<-[:p53]-()<-[:p76]-()-[:p72]->(x1), (x1)<-[:p55]-()<-[:p61]-()<-[:p59]-()-[:p78]->(x2), (x2)<-[:p78]-()-[:p57]->()-[:p81]->(x3), (x3)-[:p81*]->(x4) RETURN DISTINCT x0;