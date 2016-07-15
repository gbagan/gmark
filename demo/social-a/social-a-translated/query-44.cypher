MATCH (x0)-[:pname|pname|pname*]->(x1), (x0)-[:phasInterest*]->(x2), (x1)-[:phasInterest|phasInterest|phasInterest*]->(x3) RETURN DISTINCT x0, x1, x2, x3;
