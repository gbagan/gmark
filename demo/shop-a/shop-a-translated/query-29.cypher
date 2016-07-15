MATCH (x0)-[:ppurchaseFor*]->(x1), (x0)-[:pdescription|pkeywords*]->(x2), (x1)-[:pdescription|pcaption|pdescription*]->(x3) RETURN DISTINCT x0, x1, x2;
