MATCH (x0)-[:ppublishedIn|ppublishedIn|ppublishedIn*]->(x1), (x1)-[:ppublishedIn|ppublishedIn*]->(x2), (x2)<-[:ppublishedIn]-()-[:ppublishedIn]->(x3), (x3)-[:ppublishedIn|ppublishedIn*]->(x4) RETURN DISTINCT x0, x4;