MATCH (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x1), (x0)-[:pheldIn]->()<-[:pheldIn]-()-[:pheldIn]->()<-[:pheldIn]-(x2), (x2)-[:ppublishedIn*]->(x1) RETURN DISTINCT x0, x1;
