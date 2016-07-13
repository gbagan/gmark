MATCH (x0)<-[:pEncodedOn]-()<-[:pInteracts]-()-[:pReference]->(x1), (x0)<-[:pEncodedOn]-()-[:pHasKeyword]->(x2), (x1)<-[:pEncodedOn]-()-[:pHasKeyword]->(x3) RETURN DISTINCT x0, x1;
