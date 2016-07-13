MATCH (x0)-[:ppublishedIn*]->(x1), (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x2), (x1)-[:ppublishedIn|ppublishedIn|pheldIn*]->(x3) RETURN DISTINCT x0, x1;
