MATCH (x0)-[:ppurchaseFor*]->(x1), (x1)-[:pdirector]->()<-[:pfriendOf]-()-[:plike]->(x2), (x2)-[:phasGenre]->()-[:ptype]->(x3) RETURN DISTINCT x0, x3;
