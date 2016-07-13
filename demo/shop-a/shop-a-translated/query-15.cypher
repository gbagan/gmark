MATCH (x0)<-[:pgender]-()<-[:pfollows]-()-[:plike]->(x1), (x1)-[:pkeywords]->()<-[:pdescription]-()-[:plegalName]->()<-[:pfamilyName]-(x2), (x2)-[:previewer*]->(x3) RETURN DISTINCT x0, x3;
