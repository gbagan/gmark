MATCH (x0)-[:pname]->()<-[:pname]-()-[:pisPartOf]->(x1), (x1)-[:pname]->()<-[:pname]-(x2), (x2)-[:pname|pname|pname*]->(x3), (x3)-[:pname|pname*]->(x4) RETURN DISTINCT x2, x1, x0;
