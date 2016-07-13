MATCH (x0)<-[:ptext]-()-[:partist]->()-[:plocation]->(x1), (x0)<-[:ptext]-()-[:ptag]->(x2), (x1)<-[:pcomposer]-()-[:ptag]->(x3) RETURN DISTINCT x0, x2, x1;
