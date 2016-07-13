MATCH (x0)<-[:p9]-()-[:p65]->(x1), (x1)-[:p60*]->(x2), (x2)-[:p68|p58*]->(x3) RETURN DISTINCT x1, x2, x0;
