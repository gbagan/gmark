MATCH (x0)-[:p76]->()-[:p67]->()-[:p57]->(x1), (x1)-[:p54|p79|p76*]->(x2), (x2)-[:p81*]->(x3) RETURN DISTINCT x0, x3;
