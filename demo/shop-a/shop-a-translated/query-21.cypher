MATCH (x0)-[:partist]->()<-[:previewer]-()-[:previewer]->()<-[:pfollows]-(x1), (x1)-[:pconductor*]->(x2), (x2)-[:pfriendOf|pemployee*]->(x3) RETURN DISTINCT x2, x0, x1;
