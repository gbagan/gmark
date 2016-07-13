MATCH (x0)-[:ppublishedIn*]->(x1), (x1)-[:pheldIn]->()<-[:pheldIn]-(x2), (x2)<-[:ppublishedIn]-()-[:ppublishedIn]->()<-[:ppublishedIn]-()-[:ppublishedIn]->(x3) RETURN "true" LIMIT 1;
