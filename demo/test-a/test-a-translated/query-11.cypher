MATCH (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x1), (x0)-[:ppublishedIn*]->(x2), (x1)-[:ppublishedIn*]->(x3) RETURN DISTINCT x0;
