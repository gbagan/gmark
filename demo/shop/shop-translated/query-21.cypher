MATCH (x0)<-[:p51]-()<-[:p53]-()-[:p53]->(x1), (x0)<-[:p51]-()<-[:p53]-()-[:p66]->(x2), (x1)<-[:p51]-()-[:p52]->()<-[:p52]-()-[:p66]->(x3) RETURN DISTINCT x0;