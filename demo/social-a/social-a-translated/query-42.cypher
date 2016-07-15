MATCH (x0)-[:pname|pname*]->(x1), (x0)-[:pisLocatedIn*]->(x2), (x1)-[:pisLocatedIn|pname|pisPartOf*]->(x3) RETURN "true" LIMIT 1;
