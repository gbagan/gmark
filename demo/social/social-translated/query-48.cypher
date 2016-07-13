MATCH (x0)-[:p16*]->(x1), (x1)-[:p16|p16*]->(x2), (x0)-[:p13|p13|p13*]->(x3), (x2)-[:p13*]->(x4) RETURN "true" LIMIT 1;
