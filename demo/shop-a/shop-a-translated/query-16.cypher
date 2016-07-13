MATCH (x0)-[:phasReview]->()<-[:phasReview]-()-[:partist]->()-[:plike]->(x1), (x1)-[:pconductor*]->(x2), (x2)-[:pconductor]->()-[:pfollows]->(x3) RETURN DISTINCT x0;
