MATCH (x0)-[:pcontentRating]->()<-[:puserId]-()<-[:partist]-(x1), (x1)-[:partist]->()-[:plike]->(x2), (x2)-[:peditor]->()<-[:partist]-()<-[:ppurchaseFor]-(x3) RETURN DISTINCT x0;
