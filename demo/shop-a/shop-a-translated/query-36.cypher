MATCH (x0)-[:pincludes|pdirector*]->(x1), (x0)-[:ptitle|pcontentRating|paward*]->(x2), (x1)-[:pactor*]->(x3) RETURN DISTINCT x0;
