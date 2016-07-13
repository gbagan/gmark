MATCH (x0)-[:pdescription]->()<-[:pdescription]-(x1), (x0)-[:pcontentRating]->()<-[:pprintPage]-(x2), (x0)<-[:plike]-()-[:pfamilyName]->()<-[:ptext]-(x3) RETURN "true" LIMIT 1;
