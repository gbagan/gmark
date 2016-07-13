MATCH (x0)-[:pname*]->(x1), (x1)<-[:pworksAt]-()-[:plocationIP]->(x2), (x2)-[:planguage*]->(x3) RETURN DISTINCT x2, x0, x3, x1 UNION  UNION ;
