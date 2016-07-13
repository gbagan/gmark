MATCH (x0)-[:peditor|phomepage*]->(x1), (x1)-[:pauthor]->()-[:pfriendOf]->()<-[:pemployee]-()-[:poffers]->(x2), (x0)-[:pauthor|plike|plike*]->(x3), (x2)-[:plike|plike*]->(x4) RETURN DISTINCT x0;
