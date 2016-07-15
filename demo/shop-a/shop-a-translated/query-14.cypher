MATCH (x0)<-[:pnationality]-()<-[:pauthor]-()-[:planguage]->(x1), (x0)<-[:pnationality]-()-[:plike]->(x2), (x2)-[:phasReview|ppurchaseFor|peditor*]->(x1) RETURN DISTINCT x0, x1, x2;
