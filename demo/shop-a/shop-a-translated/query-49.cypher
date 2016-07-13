MATCH (x0)-[:pcontentRating]->()<-[:pprintColumn]-()-[:ptitle]->()<-[:pperformer]-(x1), (x1)-[:phomepage|pconductor|pincludes*]->(x2), (x2)-[:phomepage]->()-[:purl]->(x3) RETURN "true" LIMIT 1;
