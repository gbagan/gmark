MATCH (x0)-[:pname*]->(x1), (x1)-[:pname]->()<-[:plength]-()-[:phasMember]->()-[:pemail]->(x2), (x2)-[:pname|plength|pname*]->(x3) RETURN DISTINCT x2, x0, x1;
