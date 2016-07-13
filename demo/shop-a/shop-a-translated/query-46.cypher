MATCH (x0)<-[:pbirthDate]-()-[:pgender]->(x1), (x0)<-[:pexpires]-()<-[:plike]-()-[:pfollows]->()-[:pgender]->(x2), (x0)<-[:pexpires]-()-[:pauthor]->()-[:pgender]->(x3) RETURN DISTINCT x0;
