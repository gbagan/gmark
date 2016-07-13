MATCH (x0)<-[:planguage]-()-[:pexpires]->(x1), (x1)<-[:pexpires]-()-[:phasReview]->()<-[:phasReview]-(x2), (x2)-[:phasReview*]->(x3), (x3)-[:pconductor|phasReview*]->(x4) RETURN DISTINCT x2, x1, x0;
