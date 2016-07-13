MATCH (x0)-[:plike]->()<-[:plike]-()<-[:previewer]-(x1), (x1)-[:phasReview*]->(x2), (x2)-[:previewer*]->(x3), (x3)<-[:phasReview]-()-[:partist]->()<-[:partist]-(x4) RETURN DISTINCT x0;
