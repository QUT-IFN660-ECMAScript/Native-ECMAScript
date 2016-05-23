#include "./runtime/core.hpp"
#include "./runtime/console.hpp"

int main() {
	ESValue* r0 = new Reference(env, "x");
	ESValue* r1 = new Reference(env, "y");
	ESValue* r2 = new Number(5);
	ESValue* r3 = Core::plus(r1, r2);
	ESValue* r4 = Assign(r0, r3);
return 0;
}
