MATCH (x0)<-[:pisSubclassOf]-()-[:pcontent]->()<-[:pname]-(x1), (x1)<-[:pisLocatedIn]-()-[:pstudyAt]->(x2), (x2)-[:pname]->()<-[:pbrowserUsed]-()-[:pemail]->(x3) RETURN "true" LIMIT 1;
