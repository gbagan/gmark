MATCH (x0)-[:pname*]->(x1), (x0)-[:pname|pname*]->(x2), (x2)-[:pisLocatedIn]->()-[:pname]->()<-[:pname]-()-[:pbirthday]->(x1) RETURN DISTINCT x0;
