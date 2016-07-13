MATCH (x0)-[:pname|pname*]->(x1), (x1)<-[:pisPartOf]-()-[:pisPartOf]->()-[:pisPartOf]->(x2), (x0)-[:pname|pname|pname*]->(x3), (x2)-[:pname|pname*]->(x4) RETURN DISTINCT x1, x2, x0 UNION ;
