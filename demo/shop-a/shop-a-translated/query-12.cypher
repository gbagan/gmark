MATCH (x0)-[:pauthor|pauthor|pauthor*]->(x1), (x0)-[:pauthor|peditor|pauthor*]->(x2), (x1)-[:pauthor|peditor|pauthor*]->(x3) RETURN "true" LIMIT 1;
