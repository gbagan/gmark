MATCH (x0)-[:ptitle]->()<-[:pproducer]-()-[:phasReview]->(x1), (x0)-[:phasReview|previewer|phasReview*]->(x2), (x2)-[:phasReview|phasReview|phasReview*]->(x1) RETURN DISTINCT x0, x1;
