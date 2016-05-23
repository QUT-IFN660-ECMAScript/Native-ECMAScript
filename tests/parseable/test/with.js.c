include "./runtime/core.hpp"
include "./runtime/console.hpp"

int main() {
	JSValue* r0 = new Reference(env, "a");
	JSValue* r1 = new Reference(env, "radius");
	JSValue* r2 = new Number(10);
	JSValue* r3 = Assign(r1, r2);
return 0;
}
