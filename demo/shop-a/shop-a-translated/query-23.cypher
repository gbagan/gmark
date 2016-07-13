MATCH (x0)<-[:phasGenre]-()-[:planguage]->(x1), (x0)<-[:phasGenre]-()-[:planguage]->(x2), (x0)<-[:phasGenre]-()-[:pdescription]->()<-[:ptitle]-()-[:planguage]->(x3) RETURN DISTINCT x0, x1;
