MATCH (x0)-[:p75*]->(x1), (x1)-[:p30]->()<-[:p23]-()-[:p5]->()<-[:p19]-(x2), (x0)-[:p33|p27|p30*]->(x3), (x2)-[:p29|p27|p29*]->(x4) RETURN DISTINCT x0;