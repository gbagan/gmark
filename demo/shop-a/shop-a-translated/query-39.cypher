MATCH (x0)<-[:pgender]-()-[:page]->(x1), (x1)<-[:page]-()-[:plike]->()-[:pkeywords]->()<-[:pcomposer]-(x2), (x2)-[:phasReview*]->(x3) RETURN DISTINCT x0, x1, x2;
