MATCH (x0)-[:pname*]->(x1), (x1)-[:pname]->()<-[:plocationIP]-()-[:pworksAt]->(x2), (x2)-[:pname*]->(x3), (x3)-[:pname|pname|pname*]->(x4) RETURN DISTINCT x0, x4 UNION ;
