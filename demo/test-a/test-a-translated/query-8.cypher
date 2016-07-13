MATCH (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x1), (x0)-[:ppublishedIn*]->(x2), (x0)-[:ppublishedIn*]->(x3), (x0)-[:ppublishedIn*]->(x4) RETURN DISTINCT x2, x0, x1, x3;
