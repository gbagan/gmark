MATCH (x0)-[:previewer|previewer*]->(x1), (x0)-[:phasReview|previewer|previewer*]->(x2), (x0)-[:phasReview|previewer|previewer*]->(x3) RETURN DISTINCT x1, x0, x2;
