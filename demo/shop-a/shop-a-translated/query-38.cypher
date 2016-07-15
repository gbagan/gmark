MATCH (x0)<-[:pbirthDate]-()-[:plocation]->(x1), (x0)<-[:pvalidForm]-()-[:peligibleQuantity]->()<-[:popus]-()-[:pperformedIn]->(x2), (x1)<-[:pbirthDate]-()-[:plocation]->(x3) RETURN DISTINCT x0;
