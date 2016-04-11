class Node {
//public:
	//Node(){}
	//virtual 
};

class Expression:public Node{
};

class AssignmentExpression:public Expression{
private:
	Expression *lhs, *rhs;
public: AssignmentExpression(Expression *lhs, Expression *rhs){
		this->lhs = lhs;
		this->rhs = rhs;
	}
};

class IntegerLiteralExpression:public Expression{
private:
	int value;
public:
	IntegerLiteralExpression(int value){
		this->value = value;
	}
};

class IndentifierExpression:public Expression{
private:
	std::string *name;
public:
	IndentifierExpression(std::string *name){
		this->name = name;
	}
};

