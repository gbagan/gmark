MATCH (x0)-[:pgender|pbrowserUsed|plocationIP*]->(x1), (x1)-[:pbrowserUsed*]->(x2), (x0)-[:pknows|pknows|pknows*]->(x3), (x2)-[:pworksAt|pbrowserUsed|pknows*]->(x4) RETURN DISTINCT x1, x0, x2, x3;
