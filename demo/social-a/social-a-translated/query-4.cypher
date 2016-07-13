MATCH (x0)<-[:pisPartOf]-()<-[:pisLocatedIn]-()-[:pgender]->(x1), (x0)-[:pname]->()<-[:plocationIP]-(x2), (x1)-[:pname]->()<-[:pname]-()-[:pname]->()<-[:plocationIP]-(x3) RETURN DISTINCT x0;
