MATCH (x0)-[:pname]->()<-[:pname]-()-[:pisPartOf]->()-[:pisPartOf]->(x1), (x0)-[:pisLocatedIn]->()-[:pisPartOf]->(x2), (x0)-[:pisLocatedIn]->()-[:pisPartOf]->(x3) RETURN DISTINCT x0 UNION ;
