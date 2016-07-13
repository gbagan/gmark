MATCH (x0)-[:phasReview|phasReview*]->(x1), (x1)-[:previewer]->()<-[:pfollows]-()<-[:pcontactPoint]-(x2), (x2)-[:pcontactPoint|pemployee*]->(x3) RETURN DISTINCT x1, x0, x2;
