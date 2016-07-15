MATCH (x0)<-[:planguage]-()-[:ptag]->(x1), (x0)<-[:planguage]-()-[:pnumberOfPages]->()<-[:pcontentSize]-()-[:ptag]->(x2), (x0)<-[:planguage]-()-[:ptag]->(x3) RETURN DISTINCT x0, x1;
