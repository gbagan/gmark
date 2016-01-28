/*  converting queries in gMark's XML syntax to queries in LogiQL, SQL:1999, SPARQL 1.1,
 *  and Cypher syntax
 */

#include "querytranslate.h"

void qtranslate(const string & inputfilename, 
                const string & sqlfilename, 
                const string & sparqlfilename, 
                const string & cypherfilename, 
                const string & logicbloxfilename)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(inputfilename.c_str()); 
    ofstream sqlout, sparqlout, cypherout, logicbloxout;
    int qcount = 0;

    if (! result)
    {
        cout << endl << "XML file [" << inputfilename << "] parsed with errors"<< endl;
        cout << "Error description: " << result.description() << endl;
        cout << "Error offset: " << result.offset << endl << endl;
        return;
    }

    if (!sqlfilename.empty())
    {
        sqlout.open(sqlfilename);
        if (not sqlout)
        {
            cerr << sqlfilename;
            exit(EXIT_FAILURE);
        }
        cout << endl << "SQL queries written to " << sqlfilename;
    }
    if (!sparqlfilename.empty())
    {
        sparqlout.open(sparqlfilename);
        if (not sparqlout)
        {
            cerr << sparqlfilename;
            exit(EXIT_FAILURE);
        }
        cout << endl << "SPARQL queries written to " << sparqlfilename;
    }
    if (!cypherfilename.empty())
    {
        cypherout.open(cypherfilename);
        if (not cypherout)
        {
            cerr << cypherfilename;
            exit(EXIT_FAILURE);
        }
        cout << endl << "Cypher queries written to " << cypherfilename;
    }
    if (!logicbloxfilename.empty())
    {
        logicbloxout.open(logicbloxfilename);
        if (not logicbloxout)
        {
            cerr << logicbloxfilename;
            exit(EXIT_FAILURE);
        }
        cout << endl << "Logicblox queries written to " << logicbloxfilename;
    }

    for (pugi::xml_node query : doc.child("queries").children("query"))
    {
        if (!sqlfilename.empty())
            qtranslate_sql(query, sqlout);
        if (!sparqlfilename.empty())
            qtranslate_sparql(query, sparqlout);
        if (!logicbloxfilename.empty())
          qtranslate_logicblox(query, logicbloxout);

        qcount++;
    }

    if (!cypherfilename.empty())
    {
      qtranslate_cypher_doc(doc);
      for (pugi::xml_node query : doc.child("queries").children("query")) 
        qtranslate_cypher(query, cypherout);
    }

    sqlout.close();
    sparqlout.close();
    cypherout.close();
    logicbloxout.close();
    cout << endl << endl << qcount << " queries processed" << endl << endl;
}


// translate a query into SQL
void qtranslate_sql(pugi::xml_node query, ofstream & file)
{
    bool first = true;
    for (pugi::xml_node body : query.child("bodies").children("body"))
    {
        if (! first)
        {
            file << " UNION ";
        }
        qtranslate_sql_body(body, query, file);
        first = false;
    }
    file << ";" << endl;
}


void qtranslate_sql_body(pugi::xml_node body, pugi::xml_node query, ofstream & file)
{
    pugi::xml_node temp, concat;
    string constantsString;
    unordered_map < string, pair < vector <int>, vector <int> > > variables;
    int s, c;
    bool firstWith = true;

    if (body.child("conjunct").empty())
    {
        return;
    }

    variables.clear();
    constantsString = "";
    c = 0;
    for (pugi::xml_node conjunct : body.children("conjunct"))
    {
      bool star = false;
      if (conjunct.child("star"))
      {
        star = true; 
        concat = conjunct.child("star").child("disj").child("concat");
      }
      else if (conjunct.child("disj"))
      {
        concat = conjunct.child("disj").child("concat");
      }
      else
      {
          cerr << "Bad input file: malformed conjunct";
          exit(EXIT_FAILURE);
      }

      if (firstWith) 
      {
        file << "WITH RECURSIVE c" << c << "(src, trg) AS ((";
        firstWith = false;
      }
      else
        file << ", c" << c << "(src, trg) AS ((";
     
      if (star) 
        file << "SELECT edge.src, edge.src FROM edge UNION SELECT edge.trg, edge.trg FROM edge UNION ";
      
      if (!concat)
      {
          cerr << "Bad input file: malformed star";
          exit(EXIT_FAILURE);
      }

      bool firstDisj = true;
      do
      {
          s = 0;
          if (firstDisj) 
          {
            file << "SELECT s0.src";
            firstDisj = false;
          }
          else
            file << " UNION SELECT s0.src";

          if (concat.child("symbol").empty())
          {
              cerr << "Bad input file: malformed star";
              exit(EXIT_FAILURE);
          }
          string tempString;
          string edges;
          bool first = true;
          for (pugi::xml_node symbol : concat.children("symbol"))
          {
              if (first)
              {
                  tempString = "s0.label = " + (string)symbol.text().get();
                  first = false;
                  string valueedge (symbol.attribute("inverse").value());
                  if (valueedge.compare("true") == 0) 
                    edges += "(SELECT trg as src, src as trg, label FROM edge) as s0";
                  else
                    edges += "edge s0";
              }
              else
              {
                  s++;
                  tempString += " AND s" + to_string(s) + ".label = " + symbol.text().get() + " ";
                  string valueedge (symbol.attribute("inverse").value());
                  if (valueedge.compare("true") == 0) 
                    edges += ", (SELECT trg as src, src as trg, label FROM edge) as s" + to_string(s);
                  else
                    edges += ", edge s" + to_string(s);
              }
          }
          file << ", s" << s << ".trg FROM " << edges;
          file << " WHERE ";
          if (s > 0)
          {
              for (int i=0; i < s; i++)
              {
                  file << "s" << i << ".trg = s" << i+1 << ".src AND ";
              }
          }
          file << tempString;
          concat = concat.next_sibling("concat");
      } while (concat);

      file << ")";
      /*
      if (!star) 
        file << ")";
      if (star) 
        file << ") UNION SELECT head.src, tail.trg FROM c" << c << " as head, c" << c << " as tail WHERE head.trg = tail.src";
      else 
        file << ")";
      */
       
      string tempString = conjunct.attribute("src").value();
      if (tempString.length() == 0)
      {
          cerr << "Bad input file: malformed conjunct";
          exit(EXIT_FAILURE);
      }

      if (tempString[0] == '?')
      {
          variables[tempString].first.push_back(c);
      }
      else
      {
        if (constantsString.empty())
          constantsString = (string)" c" + to_string(c) + (string)".src = " + tempString;
        else
          constantsString = constantsString + (string)" AND c" + to_string(c) + (string)".src = " + tempString;
      }

      tempString = conjunct.attribute("trg").value();
      if (tempString.length() == 0)
      {
          cerr << "Bad input file: malformed conjunct";
          exit(EXIT_FAILURE);
      }

      if (tempString[0] == '?')
      {
          variables[tempString].second.push_back(c);
      }
      else
      {
          if (constantsString.empty())
            constantsString = (string)" c" + to_string(c) + (string)".trg = " + tempString;
          else
            constantsString = constantsString + (string)" AND c" + to_string(c) + (string)".trg = " + tempString;
      }

      file << ") ";
      c++;

      if (star) 
      { 
        file << ", c" << c << "(src, trg) AS (";
        file << "SELECT src, trg FROM c" << c - 1 << " UNION SELECT head.src, tail.trg FROM c" << c -1 << " as head, c" << c << " as tail WHERE head.trg = tail.src) ";
        c++;
      }
    }

    qtranslate_sql_select(query, variables, c, file, constantsString);
}


void qtranslate_sql_select(pugi::xml_node query, 
                           unordered_map < string, pair < vector <int>, vector <int> > > & variables, 
                           int c, 
                           ostream & file, 
                           const string & constantsString)
{
    vector <int>::iterator vecIt;
    int anchor;
    bool distinct = true;

    if(query.child("head").child("var").empty())
    {
        file << "SELECT \"true\" FROM edge WHERE EXISTS (SELECT *";
    }
    else
    {
        //hack for selectivity experiments
        //file << "SELECT COUNT (*) FROM ( ";

        if (distinct) 
          file << "SELECT DISTINCT ";
        else 
          file << "SELECT ";
        bool first = true;

        for (pugi::xml_node variable : query.child("head").children("var"))
        {
            auto varsIt = variables.find(variable.text().get());
            if (varsIt == variables.end())
            {
                cerr << "Bad input file: malformed head " << endl;
                exit(EXIT_FAILURE);
            }

            if (!first)
            {
                file << ", ";
            }
            if (!varsIt->second.first.empty())
            {
                file << "c" << (varsIt->second.first)[0] << ".src";
            }
            else
            {
                file << "c" << (varsIt->second.second)[0] << ".trg ";
            }
            first = false;
        }
    }

    file << " FROM ";
    for (int i=0; i<c-1; i++)
        file << "c" << i << ", ";
    file << "c" << c-1;

    bool firstWhere = true;
    for (auto & variable : variables)
    {
        if (variable.second.first.size() + variable.second.second.size() > 1)
        {
            if (firstWhere)
            {
                file << " WHERE ";
                firstWhere = false;
            }
            else
            {
                file << " AND ";
            }
            if (variable.second.first.size() > 0)
            {
                vecIt = variable.second.first.begin();
                anchor = *vecIt;
                vecIt++;
                if (vecIt != variable.second.first.end())
                {
                    file << "c" << anchor << ".src = c" << *vecIt << ".src";
                    vecIt++;
                    while (vecIt !=  variable.second.first.end())
                    {
                        file << " AND c" << anchor << ".src = c" << *vecIt << ".src";
                        vecIt++;
                    }
                }
                else
                {
                    vecIt = variable.second.second.begin();
                    file << "c" << anchor << ".src = c" << *vecIt << ".trg";
                    vecIt++;
                    while (vecIt !=  variable.second.second.end())
                    {
                        file << " AND c" << anchor << ".src = c" << *vecIt << ".trg";
                        vecIt++;
                    }
                }
            }
            else
            {
                vecIt = variable.second.second.begin();
                anchor = *vecIt;
                vecIt++;
                if (vecIt !=  variable.second.second.end())
                {
                    file << "c" << anchor << ".trg = c" << *vecIt << ".trg";
                    vecIt++;
                    while (vecIt !=  variable.second.second.end())
                    {
                        file << " AND c" << anchor << ".trg = c" << *vecIt << ".trg";
                        vecIt++;
                    }
                }
            }
        }
    }

    if (!constantsString.empty())
    {
      if(query.child("head").child("var").empty())
      {
        if (firstWhere) 
          file << " WHERE" << constantsString << ")";
        else 
          file << " AND" << constantsString << ")";
      }
      else 
      {
        if (firstWhere) 
          file << " WHERE" << constantsString;
        else 
          file << " AND" << constantsString;
      }
    }
    else if(query.child("head").child("var").empty())
      file << ")";

  //hack for selectivity experiments
  //if(!query.child("head").child("var").empty()) 
  //  file << ") as hack";
}


void qtranslate_sparql_regex(pugi::xml_node query, ofstream & file)
{
    string name = query.name();
    if(name == "symbol")
    {
        if (query.attribute("inverse").as_bool())
        {
            file << "^";
        }
        file << ":p" << query.text().get();
    }
    else if(name == "star")
    {
        file << "(";
        qtranslate_sparql_regex(query.first_child(), file);
        file << "){,3}";
    }
    else if (name == "concat")
    {
        bool first = true;
        file << "(";
        for (pugi::xml_node child : query.children())
        {
            if (!first)
                file << "/";
            qtranslate_sparql_regex(child, file);
            first = false;
        }
        file << ")";
    }
    else if (name == "disj")
    {
        bool first = true;
        file << "(";
        for (pugi::xml_node child : query.children())
        {
            if (!first)
                file << "|";
            qtranslate_sparql_regex(child, file);
            first = false;
        }
        file << ")";
    }

}

string node_to_sparql(const string & name)
{
    if(name.size() > 0 && name[0] == '?')
    {
        return name;
    }
    else
    {
        return '"' + name + '"';
    }
}

// translate a query into SPARQL 1.1
void qtranslate_sparql(pugi::xml_node query, ofstream & file)
{
    bool first = true;
    bool booleanquery = true;
    bool distinct = true;

    file << "PREFIX : <http://example.org/gmark/> ";
    //hack for selectivity experiments
    //file << "SELECT (COUNT(*) AS ?count) { ";
    
    for (pugi::xml_node var_node : query.child("head").children("var"))
    {
        if (distinct && first)
        {
            distinct = false;
            booleanquery = false;
            first = false;
            file << "SELECT DISTINCT";
        }
        else if (first)
        {
            booleanquery = false;
            first = false;
            file << "SELECT";
        }
        file << " " << var_node.text().get();
    }
    if (!first)
    {
      first = true;
      file << " WHERE { ";
    }
    if (booleanquery)
    {
        file << "ASK { ";
    }
    
    for (pugi::xml_node body_node : query.child("bodies").children("body"))
    {
        if(!first)
        {
            file << " UNION ";
        }
        file << " { ";
        for (pugi::xml_node conjunct_node : body_node.children("conjunct"))
        {
            file << " " << node_to_sparql(conjunct_node.attribute("src").value()) << " ";
            qtranslate_sparql_regex(conjunct_node.first_child(), file);
            file << " " << node_to_sparql(conjunct_node.attribute("trg").value()) << " .";
        }
        file << " } ";
        first = false;
    }

    file << "}" << endl;
    //hack for selectivity experiments
    //file << "} }" << endl;

}


// translate one rule body into Cypher
// NOTE: see comments for qtranslate_cypher()
//
void qtranslate_cypher_body(pugi::xml_node body, string outputVariables, ofstream & file)
{
  int c = 0;
  bool distinct = true;
  bool first = true;
  bool firstSymbol = true;
  bool star = false;
  string whereClause = "";
  string srcVar = "";
  string trgVar = "";
  pugi::xml_node currentNode, currentSymbol;

  if (body.child("conjunct").empty()) 
    return;

  file << "MATCH ";

  for (pugi::xml_node conjunct : body.children("conjunct"))
  {
      if (! first)
        file << ", ";

      // process source and target of conjunct
      srcVar = conjunct.attribute("src").value();
      trgVar = conjunct.attribute("trg").value();
      if (srcVar.length() == 0 || trgVar.length() == 0)
      {
          cerr << "Bad input file: malformed conjunct";
          exit(EXIT_FAILURE);
      }
      if (srcVar[0] != '?')
      {
        // src is a constant
        if (whereClause.length() == 0) 
          whereClause = "id(gmarkvariable" + srcVar + ") = " + srcVar;
        else
          whereClause += " AND id(gmarkvariable" + srcVar + ") = " + srcVar;
      }
      if (trgVar[0] != '?')
      {
        // trg is a constant
        if (whereClause.length() == 0) 
          whereClause = "id(gmarkvariable" + trgVar + ") = " + trgVar;
        else
          whereClause += " AND id(gmarkvariable" + trgVar + ") = " + trgVar;
      }

      // process the conjunct's predicate
      star = false;
      if (conjunct.child("star"))
      {
        currentNode = conjunct.child("star").child("disj").child("concat");
        star = true;
      }
      else if (conjunct.child("disj"))
      {
        currentNode = conjunct.child("disj").child("concat");
      }
      else
      {
        cerr << "Bad input file: malformed conjunct";
        exit(EXIT_FAILURE);
      }

      if (!currentNode)
      {
          cerr << "Bad input file: malformed star";
          exit(EXIT_FAILURE);
      }

      if (star)
      {
        // (s)-[r*]->(t) 
        // WHERE ALL (x in type(r) WHERE x=type1 OR ... OR x=typeN)
        file << "(" << ((srcVar[0] == '?') ? srcVar.substr(1) : "gmarkvariable" + srcVar) << ")-[";

        firstSymbol = true;
        do
        {
          if (! firstSymbol) 
            file << "|p" << (string)currentNode.child("symbol").text().get();
          else
            file << ":p" << (string)currentNode.child("symbol").text().get();

          firstSymbol = false;
          currentNode = currentNode.next_sibling("concat"); 
        } while (currentNode);

        file << "*]->(" << ((trgVar[0] == '?') ? trgVar.substr(1) : "gmarkvariable" + trgVar) << ")";

        /*
        if (whereClause.length() == 0) 
          whereClause = "ALL (x IN RELATIONSHIPS(gmarkpath" + to_string(c) + ") WHERE";
        else
          whereClause += " AND ALL (x IN RELATIONSHIPS(gmarkpath" + to_string(c) + ") WHERE";

        firstSymbol = true;
        do
        {
          if (! firstSymbol) 
            whereClause += " OR TYPE(x)=p" + (string)currentNode.child("symbol").text().get();
          else
            whereClause += " TYPE(x)=p" + (string)currentNode.child("symbol").text().get();

          firstSymbol = false;
          currentNode = currentNode.next_sibling("concat"); 
        } while (currentNode);
        whereClause += ")";
        */

        c++;
      }
      else
      {
        currentSymbol = currentNode.child("symbol");
        if (!currentSymbol)
        {
          cerr << "Bad input file: malformed conjunct structure";
          exit(EXIT_FAILURE);
        }

        firstSymbol = true;
        file << "(" << ((srcVar[0] == '?') ? srcVar.substr(1) : "gmarkvariable" + srcVar) << ")";
        do 
        {
          string valueedge (currentSymbol.attribute("inverse").value());
          if (valueedge.compare("true") == 0) 
          {
            // ()<-[p]-() 
            if (! firstSymbol) 
            {
              file << "()<-[:p" << currentSymbol.text().get() << "]-";
            }
            else
            {
              file << "<-[:p" << currentSymbol.text().get() << "]-";
              firstSymbol = false;
            }
          }
          else 
          {
            // ()-[p]->() 
            if (! firstSymbol) 
            { 
              file << "()-[:p" << currentSymbol.text().get() << "]->";
            }
            else
            {
              file << "-[:p" << currentSymbol.text().get() << "]->";
              firstSymbol = false;
            }
          }
          currentSymbol = currentSymbol.next_sibling("symbol"); 
        } while (currentSymbol);
        file << "(" << ((trgVar[0] == '?') ? trgVar.substr(1) : "gmarkvariable" + trgVar) << ")";
      }
      first = false;
  }

  if(whereClause.length() > 0) 
    file << " WHERE " << whereClause;

  if(outputVariables.length() == 0) 
  {
    // a boolean query
    file << " RETURN \"true\" LIMIT 1";
  }
  else
  {
    if (distinct) 
    {
      file << " RETURN DISTINCT " << outputVariables;
      //hack for selectivity experiments
      //file << " RETURN count(*)";

    }
    else 
      file << " RETURN " << outputVariables;

  }
}


// convert queries in input XML doc into restricted syntax where all non-recursive
// conjuncts have only one disjunct (of arbitrary path length)
void qtranslate_cypher_doc(pugi::xml_document & doc)
{
  pugi::xml_node copy, querycopy, newbody; 
  pugi::xml_document tempDoc;
  vector < pugi::xml_node > conjunctList;
  vector < pugi::xml_node > removeList;

  tempDoc.append_child("queries");
  tempDoc.append_child("scratch");

  for (pugi::xml_node query: doc.child("queries").children("query"))
  { 
    querycopy = tempDoc.child("queries").append_copy(query);
    while (querycopy.child("bodies").remove_child("body"));

    for (pugi::xml_node body : query.child("bodies").children("body"))
    {
      conjunctList.clear();
      newbody = tempDoc.child("scratch").append_child("body");

      for (pugi::xml_node conjunct : body.children("conjunct"))
      {
        if (conjunct.child("disj")) 
        { 
          copy = tempDoc.child("scratch").append_copy(conjunct);
          conjunctList.push_back(copy);
        }
        else
          newbody.append_copy(conjunct);
      }

      qtranslate_cypher_doc_reconstructBody(querycopy, newbody, conjunctList);
    }
  }

  doc.remove_child("queries");
  doc.append_copy(tempDoc.child("queries"));
}

void qtranslate_cypher_doc_reconstructBody(pugi::xml_node & query, 
                                           pugi::xml_node & body, 
                                           vector < pugi::xml_node > conjunctList)
{ 
  pugi::xml_node current, currentcopy, disjunct, bodycopy; 
  pugi::xml_document tempDoc;

  if (conjunctList.empty()) 
  {
    query.child("bodies").append_copy(body);
  }
  else
  {
    current = conjunctList.back();
    conjunctList.pop_back();
    currentcopy = tempDoc.append_copy(current);
    while (currentcopy.child("disj").remove_child("concat"));
    for (pugi::xml_node concat : current.child("disj").children("concat"))
    {
      disjunct = currentcopy.child("disj").append_copy(concat);
      bodycopy = body.append_copy(currentcopy);
      qtranslate_cypher_doc_reconstructBody(query, body, conjunctList);
      body.remove_child(bodycopy);
      currentcopy.child("disj").remove_child(disjunct);
    }
  }
}

// translate a query into Cypher
//
// NOTE: Cypher only supports kleene star on a disjunction of single edge labels, i.e., of
// the form (a1 + ... + an)*, where each ai is a single symbol.  Furthermore, inverse
// labels can not appear under Kleene star.  (see email exchange of June 2015 with Tobias
// Lindaaker of Neo Technology)
//
void qtranslate_cypher(pugi::xml_node query, ofstream & file) 
{
  bool first = true;
  string outputVariables = "";
  string oneVar;

  // collect projection list
  for (pugi::xml_node var_node : query.child("head").children("var"))
  {
    if (first)
    {
      oneVar = var_node.text().get();
      outputVariables +=  oneVar.substr(1);
      first = false;
    }
    else
    {
      oneVar = var_node.text().get();
      outputVariables +=  ", " + oneVar.substr(1);
    }
  }

  first = true;

  // a union over bodies
  // for each body, construct a MATCH-WHERE-RETURN statement
  for (pugi::xml_node body : query.child("bodies").children("body"))
  {
      if (! first)
        file << " UNION ";
      else
        first = false;

      qtranslate_cypher_body(body, outputVariables, file);
  }
  file << ";" << endl;
}


// translate a query into LogicBlox's LogiQL
void qtranslate_logicblox(pugi::xml_node query, ofstream & file)
{
  bool first = true;
  //string head = "_(";
  string head = "query(";
  string oneVar;
  int bodyIndex = 0;

  // collect projection list
  for (pugi::xml_node var_node : query.child("head").children("var"))
  {
    if (first)
    {
      oneVar = var_node.text().get();
      head +=  oneVar.substr(1);
      first = false;
    }
    else
    {
      oneVar = var_node.text().get();
      head +=  ", " + oneVar.substr(1);
    }
  }
  head +=  ") <- ";

  file << "BEGIN";
  for (pugi::xml_node body : query.child("bodies").children("body"))
  { 
    qtranslate_logicblox_body(body, bodyIndex, file, head);
    bodyIndex++;
  }
  file << endl << "END" << endl;
}


void qtranslate_logicblox_body(pugi::xml_node body, int bodyIndex, ofstream & file, string head)
{
  string mainbody = "";
  int conjunctIndex = 0;
  bool first = true;
  bool star = false;
  string srcVar = "";
  string trgVar = "";
  string predicate;
  pugi::xml_node concat;

  for (pugi::xml_node conjunct : body.children("conjunct"))
  {
      // process source and target of conjunct
      srcVar = conjunct.attribute("src").value();
      trgVar = conjunct.attribute("trg").value();
      if (srcVar.length() == 0 || trgVar.length() == 0)
      {
          cerr << "Bad input file: malformed conjunct";
          exit(EXIT_FAILURE);
      }

      // process the conjunct's predicate
      star = false;
      if (conjunct.child("star"))
      {
        concat = conjunct.child("star").child("disj").child("concat");
        star = true;
      }
      else if (conjunct.child("disj"))
      {
        concat = conjunct.child("disj").child("concat");
      }
      else
      {
        cerr << "Bad input file: malformed conjunct structure";
        exit(EXIT_FAILURE);
      }

      if (!concat)
      {
          cerr << "Bad input file: malformed structure";
          exit(EXIT_FAILURE);
      }
        
      predicate = "gmarkSubquery" + to_string(bodyIndex) + to_string(conjunctIndex);

      do
      {
        file << endl << predicate << "(x, y) <- ";
        bool firstSymbol = true;
        int i = 0;
        for (pugi::xml_node symbol : concat.children("symbol"))
        {
          if (firstSymbol)
          {
            firstSymbol = false;
            file << "core:edge:edge(";
          }
          else
            file << ", core:edge:edge(";

          string valueedge = symbol.attribute("inverse").value();
          if (valueedge.compare("true") == 0) 
          {
            i++;
            file << "x" << to_string(i) << ", ";
            file << (string)symbol.text().get() << ", ";
            file << "x" << to_string(i - 1) << ")";
          }
          else
          {
            file << "x" << to_string(i) << ", ";
            file << (string)symbol.text().get() << ", ";
            i++;
            file << "x" << to_string(i) << ")";
          }
        }
        file << ", x = x0";
        file << ", y = x" << to_string(i);
        file << ".";

        concat = concat.next_sibling("concat"); 
      } while (concat);

      if (star) 
      {
        file << endl << predicate << "(x, x) <- core:edge:edge(x, r, y).";
        file << endl << predicate << "(y, y) <- core:edge:edge(x, r, y).";
        file << endl << predicate << "(x, y) <- " << predicate << "(x, z), " << predicate << "(z, y).";
      }
      if (first) 
      {
        mainbody +=  predicate;
        mainbody +=  "(";
      }
      else
      {
        mainbody +=  ", ";
        mainbody +=  predicate;
        mainbody +=  "(";
      }
      mainbody += ((srcVar[0] == '?') ? srcVar.substr(1) : srcVar);
      mainbody += ", ";
      mainbody += ((trgVar[0] == '?') ? trgVar.substr(1) : trgVar);
      mainbody +=  ")";

      conjunctIndex++;
      first = false;
  }

  file << endl << head << mainbody << ".";
}
