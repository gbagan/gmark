MATCH (x0)-[:pincludes|ppurchaseFor*]->(x1), (x1)-[:pcaption]->()<-[:pcaption]-()-[:pdescription]->()<-[:pkeywords]-(x2), (x2)-[:pactor|phasReview*]->(x3) RETURN DISTINCT x2, x1, x0, x3;
