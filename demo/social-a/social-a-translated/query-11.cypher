MATCH (x0)-[:pname*]->(x1), (x1)-[:pname|pname|pname*]->(x2), (x2)-[:pisLocatedIn]->()-[:pname]->()<-[:pname]-()<-[:pisPartOf]-(x3) RETURN DISTINCT x0, x3;
