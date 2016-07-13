MATCH (x0)-[:plocationIP|pbrowserUsed|pname*]->(x1), (x0)-[:pbrowserUsed*]->(x2), (x0)-[:pbrowserUsed|pname*]->(x3), (x0)-[:pimageFile|pbrowserUsed*]->(x4) RETURN DISTINCT x2, x1, x0 UNION  UNION ;
