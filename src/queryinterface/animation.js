window.addEventListener("load", add_events, false); 

function add_events() { 
	document.getElementById("sparql-checkbox").addEventListener("click", click_sparql, false);
	document.getElementById("opencypher-checkbox").addEventListener("click", click_opencypher, false);
	document.getElementById("sql-checkbox").addEventListener("click", click_sql, false);
	document.getElementById("datalog-checkbox").addEventListener("click", click_datalog, false);
}

function click_sparql() {
	if (document.getElementById("sparql-checkbox").checked == true) {
		document.getElementById("sparql-header").style.visibility = 'visible'; 
		document.getElementById("sparql-syntax").style.visibility = 'visible'; 
	}
	else {
		document.getElementById("sparql-header").style.visibility = 'hidden'; 
		document.getElementById("sparql-syntax").style.visibility = 'hidden'; 
	}
}

function click_opencypher() {
	if (document.getElementById("opencypher-checkbox").checked == true) {
		document.getElementById("opencypher-header").style.visibility = 'visible'; 
		document.getElementById("opencypher-syntax").style.visibility = 'visible'; 
	}
	else {
		document.getElementById("opencypher-header").style.visibility = 'hidden'; 
		document.getElementById("opencypher-syntax").style.visibility = 'hidden'; 
	}
}

function click_sql() {
	if (document.getElementById("sql-checkbox").checked == true) {
		document.getElementById("sql-header").style.visibility = 'visible'; 
		document.getElementById("sql-syntax").style.visibility = 'visible'; 
	}
	else {
		document.getElementById("sql-header").style.visibility = 'hidden'; 
		document.getElementById("sql-syntax").style.visibility = 'hidden'; 
	}
}

function click_datalog() {
	if (document.getElementById("datalog-checkbox").checked == true) {
		document.getElementById("datalog-header").style.visibility = 'visible'; 
		document.getElementById("datalog-syntax").style.visibility = 'visible'; 
	}
	else {
		document.getElementById("datalog-header").style.visibility = 'hidden'; 
		document.getElementById("datalog-syntax").style.visibility = 'hidden'; 
	}
}
