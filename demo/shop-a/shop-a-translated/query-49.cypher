MATCH (x0)<-[:ptitle]-()-[:pkeywords]->()<-[:pfamilyName]-()-[:pgender]->(x1), (x0)<-[:pemail]-()-[:pgender]->(x2), (x0)<-[:pemail]-()-[:pfollows]->()-[:pgender]->(x3) RETURN DISTINCT x0, x1;
