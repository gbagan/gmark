MATCH (x0)-[:ppublishedIn*]->(x1), (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x2), (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x3) RETURN DISTINCT x1, x2, x0, x3;
