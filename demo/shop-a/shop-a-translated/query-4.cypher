MATCH (x0)-[:phasReview*]->(x1), (x1)-[:pactor]->()<-[:previewer]-(x2), (x2)-[:previewer|phasReview*]->(x3) RETURN "true" LIMIT 1;
