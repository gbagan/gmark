MATCH (x0)<-[:pcontentSize]-()-[:ptag]->(x1), (x0)<-[:ptelephone]-()-[:ptelephone]->()<-[:pcontentRating]-()-[:ptag]->(x2), (x0)<-[:pcontentRating]-()-[:ptag]->(x3) RETURN "true" LIMIT 1;
