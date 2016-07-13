MATCH (x0)-[:pname|pname|pname*]->(x1), (x1)-[:pisLocatedIn]->()<-[:pisLocatedIn]-()-[:pisLocatedIn]->(x2), (x2)-[:pisPartOf]->()-[:pname]->()<-[:pname]-(x3) RETURN "true" LIMIT 1;
