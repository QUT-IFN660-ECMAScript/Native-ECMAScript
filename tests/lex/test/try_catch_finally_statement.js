/*
case 1: try statement
*/
var x = 0;
try {
	x = 5 / 0;
} catch (exception) {
}

//case 2
try {
	x = 5.5;
	allert(x);
} catch (exception) {
	x = x + 20;
} finally {
	x = x - 15.5;
}

//case 3
try{}finally{}