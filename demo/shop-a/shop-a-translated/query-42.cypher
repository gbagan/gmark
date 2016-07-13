MATCH (x0)-[:phomepage|pincludes|pconductor*]->(x1), (x0)-[:pmovement|pkeywords|pmovement*]->(x2), (x1)-[:pcomposer*]->(x3) RETURN "true" LIMIT 1;
