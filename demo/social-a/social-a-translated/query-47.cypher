MATCH (x0)-[:pcontent]->()<-[:pbrowserUsed]-()-[:plocationIP]->()<-[:pname]-(x1), (x1)-[:pname*]->(x2), (x2)<-[:pstudyAt]-()-[:phasInterest]->()-[:phasType]->(x3) RETURN DISTINCT x1, x2, x0;
