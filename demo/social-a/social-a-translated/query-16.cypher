MATCH (x0)-[:pimageFile|planguage*]->(x1), (x1)-[:pcontent*]->(x2), (x2)<-[:pcontainerOf]-()-[:phasMember]->()-[:plocationIP]->(x3) RETURN DISTINCT x0, x3, x2, x1;
