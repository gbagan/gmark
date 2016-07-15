MATCH (x0)<-[:pbrowserUsed]-()<-[:pisSubclassOf]-(x1), (x0)<-[:pgender]-()-[:pname]->()<-[:pname]-(x2), (x1)<-[:pgender]-()-[:pbrowserUsed]->()<-[:pname]-(x3) RETURN DISTINCT x0, x1;
