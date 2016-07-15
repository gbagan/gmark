MATCH (x0)-[:pcontactPoint*]->(x1), (x1)-[:pcontactPoint]->()-[:pfriendOf]->()-[:plike]->(x2), (x2)-[:phomepage*]->(x3) RETURN DISTINCT x0, x3;
