MATCH (x0)-[:pauthors]->()-[:ppublishedIn]->()-[:pheldIn]->(x1), (x0)-[:pauthors]->()<-[:pauthors]-(x2), (x2)-[:pauthors]->()-[:ppublishedIn]->()-[:pheldIn]->(x1) RETURN DISTINCT x0, x1;
