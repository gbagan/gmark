MATCH (x0)-[:pname|pname|pname*]->(x1), (x0)-[:pworksAt*]->(x2), (x1)-[:pworksAt|pworksAt|pworksAt*]->(x3) RETURN DISTINCT x3, x1, x0, x2;
