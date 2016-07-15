MATCH (x0)-[:pmakesPurchase]->()-[:ppurchaseFor]->(x1), (x1)-[:ptitle]->()<-[:ppaymentAccepted]-()-[:poffers]->(x2), (x2)-[:pincludes]->()-[:ptag]->(x3) RETURN DISTINCT x0, x3;
