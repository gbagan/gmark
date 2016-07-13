MATCH (x0)-[:ppublishedIn*]->(x1), (x0)-[:ppublishedIn|ppublishedIn*]->(x2), (x0)-[:ppublishedIn*]->(x3), (x0)-[:ppublishedIn|ppublishedIn*]->(x4) RETURN DISTINCT x0;
