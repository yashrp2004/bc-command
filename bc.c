#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define PLUS 1
#define MINUS -1
#define SCALE 31
#define OPERAND	100
#define OPERATOR 200
#define END	300
#define ERR 400


//ADT Number - Doubly Linked List
typedef struct node {
	int num;
	struct node *next, *prev;
}node;

typedef struct number {
	int sign, dec;
	node *head, *tail;
}number;


// Stack implementation to store Number list
typedef struct inode {
	number *a;
	struct inode *p;
}inode;
typedef inode *istack; 

typedef struct cnode {
	char a;
	struct cnode *p;
}cnode;
typedef cnode *cstack;

void iinit(istack *s) {
	*s = NULL;
}
void ipush (istack *s, number *t) {
	istack temp;
	temp = (inode *)malloc(sizeof(inode));
	temp->a = t;
	temp->p = *s;
	*s = temp;
}
number *ipop(istack *s) {
	number *t;
	istack temp;
	t = (*s)->a;
	temp = *s;
	*s = (*s)->p;
	free(temp);
	return t;
}
int iisempty (istack *s) {
	return *s == NULL;
}

//  Stack implementation to store Character Operators.
void cinit(cstack *s) {
	*s = NULL;
}
void cpush (cstack *s, char t) {
	cstack temp;
	temp = (cnode *)malloc(sizeof(cnode));
	temp->a = t;
	temp->p = (*s);
	*s = temp;
}
char cpop(cstack *s) {
	char t;
	cstack temp;
	temp = (*s);
	t = (*s)->a;
	*s = (*s)->p;
	free(temp);
	return t;
}
int cisempty (cstack *s) {
	return *s == NULL;
}

char ctop(cstack *s) {
	char ch;
	ch = cpop(s);
	cpush(s, ch);
	return ch;
}


void initNumber(number *a) {
	a->head = a->tail = NULL;
	a->dec = 0;
	a->sign = PLUS;
}
void addDigit(number *a, char ch) {
	if(ch < '0' || ch > '9')
		return;
	node *tmp = (node *)malloc(sizeof(node));
	if(tmp == NULL)
		return;
	tmp->num = ch - '0';
	tmp->next = NULL;
	if((a->head == NULL) && (a->tail == NULL)) {
		tmp->prev = NULL;
		a->head = tmp;
		a->tail = tmp;
		return;
	}
	tmp->prev = a->tail;
	a->tail->next = tmp;
	a->tail = tmp;
	return;
}

int length(number c) {
	node *p;
       	p = c.head;
	int count = 0;
	while(p != NULL) {
		count++;
		p = p->next;
	}
	return count;
}

void appendleft(number *c, int no) {
	if(no < 0 || no > 9)
		return;
	node *tmp = (node *)malloc(sizeof(node));
	if(!tmp)
		return;
	tmp->num = no;
	tmp->prev = NULL;
	if((c->head == NULL) && (c->tail == NULL)){
		tmp->next = NULL;
		c->head = c->tail = tmp;
		return;
	}
	tmp->next = c->head;
	c->head->prev = tmp;
	c->head = tmp;
	return;
}

void remov(number *a, int pos){
	int i;
	node *p, *tmp;
	/* handle errors on position */
	if(pos < 0 || pos >= length(*a))
		return;

	p = a->head;
	for(i = 0; i < pos - 1; i++)
		p = p->next;
	if(a->head == NULL) {
		return;
	}
	if(length(*a) == 1) {
		a->head = a->tail = p = NULL;
		return;
	}
	if(pos == 0) {
		p->next->prev = NULL;
		a->head = p->next;
		free(p);
		return;
	}
	if(pos == length(*a) - 1) {
		tmp = p->next;
		p->next = NULL;
		a->tail = p;
		free(tmp);
		return;
	}

	tmp = p->next;
	p->next = p->next->next;
	tmp->next->prev = p;
	free(tmp);
	return;
}
//Function to remove extra zeroes on left of number.
void zeroRemov(number *a) {
	node *p = a->head;
	int i, len = length(*a);
	for(i = 0; i < len - a->dec -1; i++) {
		if(p->num == 0) {
			a->head = p->next;
			p->next = NULL;
			a->head->prev = NULL;
			free(p);
			p = a->head;
		}else {
			break;
		}
	}
	p = a->tail;
	int decimal = a->dec;
	for(i = 0; i < decimal - 1; i++) {
		if(p->num == 0) {
			a->tail = p->prev;
			p->prev = NULL;
			a->tail->next = NULL;
			free(p);
			p = a->tail;
			a->dec--;

		}else {
			break;
		}
	}
	return;
}
void printNumber(number c) {
	node *p;
	int pos = 0;
	zeroRemov(&c);
	p = c.head;
	if(c.sign == MINUS) {
		printf("-");
		c.sign = PLUS;
	}
	while(p != NULL) {
		pos++;
		if(pos == (length(c) - c.dec + 1))
			printf(".");
		printf("%d", p->num);
		p = p->next;
	}
	printf("\n");
}

//Functions for the operations

int compareEqual(number a, number b);
void decimalEqual(number *a, number *b);
void lengthEqual(number *a, number *b);
int zeroNumber(number a);
void copy(number *a, number *b);
number *add(number *a, number *b);
number *sub(number *a, number *b);
number *mult(number *a, number *b);
number *division(number *a, number *b);


void decimalEqual(number *a, number *b) {
	int i;
	if(a->dec > b->dec) {
		int diff = a->dec - b->dec;
		for(i = 0; i < diff; i++) {
			addDigit(b, '0');
			b->dec++;
		}
	}
	else if(b->dec > a->dec) {
		int diff = b->dec - a->dec;
		for(i = 0; i < diff; i++) {
			addDigit(a, '0');
			a->dec++;
		}
	}
}

//Equal length of 2 numbers
void lengthEqual(number *a, number *b) {
	int gap;
	gap = length(*a) - length(*b);
	if(gap > 0) {
		int i = 0;
		while(i < gap) {
			appendleft(b, 0);
			i++;
		}
	}
	else if(gap < 0) {
		int i = 0;
		gap = -gap;
		while(i < gap) {
			appendleft(a, 0);
			i++;
		}
	}
}

//Check if Number is Zero or not
int zeroNumber(number a) {
	int i, flag = 0;
	node *p = a.head;
	for(i = 0; i < length(a); i++) {
		if(p->num != 0)
			flag = 1;
		 p = p->next;
	}
	return flag;
}

//copy number a in number b.
void copy(number *a, number *b) {
	int i, no, len;
	char ch;
	len = length(*a);
	node *t1 = a->head;
	for(i = 0; i < len; i++) {
		no = t1->num;
		ch = no + '0';
		addDigit(b, ch);
		t1 = t1->next;
	}
	b->dec = a->dec;
	b->sign = a->sign;
	return;
}

//Function To Compare Equal Length Numbers
int compareEqual(number a, number b) {
	lengthEqual(&a, &b);
	decimalEqual(&a, &b);
	node *p, *q;
	int len;
	int i;
	len = length(a);
	p = a.head;
	q = b.head;
	for(i = 1; i <= len; i++) {
		if(p->num > q->num)
			return 1;
		else if(p->num < q->num)
			return -1;
		p = p->next;
		q = q->next;
	}
	return 0;
}

//Addition Operation
number *add(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	decimalEqual(a, b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = sub(b, a);
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = sub(a, b);
		}
	}
	else if(a->sign == b->sign) {
		int i, n1, n2, carry = 0, sum;
		int len_a, len_b;
		node *t1 = a->tail;
		node *t2 = b->tail;
		len_a = length(*a);
		len_b = length(*b);
		if(a->sign == MINUS)
			ans->sign = MINUS;
		else
			ans->sign = PLUS;
		if(len_a >= len_b) {
			for(i = 1; i <= len_b; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_a - len_b; i++) {
				n1 = t1->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
			}
		}else {
			for(i = 1; i <= len_a; i++) {
				n1 = t1->num;
				n2 = t2->num;
				sum = n1 + n2 +carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t1 = t1->prev;
				t2 = t2->prev;
			}
			for(i = 1; i <= len_b - len_a; i++) {
				n1 = t2->num;
				sum = n1 + carry;
				carry = sum / 10;
				sum = sum % 10;
				appendleft(ans, sum);
				t2 = t2->prev;
			}
		}
		ans->dec = a->dec;
		if(carry != 0)
			appendleft(ans, carry);
	}
	return ans;
}


//Subtraction operation.
number *sub(number *a, number *b) {
	number *ans;
	ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	decimalEqual(a, b);
	lengthEqual(a, b);
	if(a->sign != b->sign) {
		if(a->sign == MINUS) {
			a->sign = PLUS;
			ans = add(a, b);
			ans->sign = MINUS;
		}
		else if(b->sign == MINUS) {
			b->sign = PLUS;
			ans = add(a, b);
			ans->sign = PLUS;
		}
	}
	else if(a->sign == b->sign) {
		if(a->sign == MINUS) {
			a->sign = b->sign = PLUS;
			ans = sub(b, a);
		}
		else if(a->sign == PLUS) {
			int n1, n2, diff, borrow = 0, i, len;
			node *t1 = a->tail;
			node *t2 = b->tail;
			len = length(*b);
			if(compareEqual(*a, *b) == 1) {
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n1 = n1 - borrow;
					if(n1 >= n2) {
						diff = n1 - n2;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n1 = n1 + 10;
						diff = n1 - n2;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}
			}
			else if(compareEqual(*a, *b) == -1) {
				ans->sign = MINUS;
				for(i = 1; i <= len; i++) {
					n1 = t1->num;
					n2 = t2->num;
					n2 = n2 - borrow;
					if(n2 >= n1) {
						diff = n2 - n1;
						borrow = 0;
						appendleft(ans, diff);
					}
					else {
						n2 = n2 + 10;
						diff = n2 - n1;
						borrow = 1;
						appendleft(ans, diff);
					}
					t1 = t1->prev;
					t2 = t2->prev;
				}
			}
			else {
				if(compareEqual(*a, *b) == 0) {
					appendleft(ans, 0);
				}
			}
		}
	}
	ans->dec = a->dec;
	return ans;
}

//Multiplication Operation.
number *mult(number *a, number *b) {
	number *ans = (number *)malloc(sizeof(number));
	initNumber(ans);
	if((zeroNumber(*a) == 0) || (zeroNumber(*b) == 0)) {
		addDigit(ans, '0');
		return ans;
	}
	int lengthdiff;
	if(a->sign == b->sign) {
		ans->sign = PLUS;
		a->sign = b->sign = PLUS;
	}
	else {
		ans->sign = MINUS;
		a->sign = b->sign = PLUS;
	}
	lengthdiff = length(*a) - length(*b);
	if(lengthdiff < 0) {
		ans = mult(b, a);
		return ans;
	}
	else {
		node *t1, *t2;
		int len_a = length(*a);
		int len_b = length(*b);
		int i, j, n1 = 0, n2 = 0;
		int tempresult[2 * len_a];
		for(i = 0; i < 2 *len_a; i++)
			tempresult[i] = 0;
		int k = 2 * len_a - 1;
		t2 = b->tail;
		for(i = 0; i < len_b; i++) {
			t1 = a->tail;
			int carry1 = 0, carry2 = 0;
			for(j = k - i; j > len_a - 2; j--) {
				if(t1 != NULL && t2 != NULL) {
					n1 = t1->num * t2->num + carry1;
					t1 = t1->prev;
					carry1 = n1 / 10;
					n1 = n1 % 10;
					n2 = tempresult[j] + n1 + carry2;
					carry2 = n2 / 10;
					n2 = n2 % 10;
					tempresult[j] = n2;
				}
				else {
					break;
				}
			}
			tempresult[j] = carry1 + carry2 + tempresult[j];
			len_a--;
			t2 = t2->prev;
		}
		for(i= k; i >= len_a - 1 && i >= 0; i--) {
			appendleft(ans, tempresult[i]);
		}
		ans->dec = a->dec + b->dec;
		return ans;
	}
}

//Division Operation.
number *division(number *m, number *n){
	if(zeroNumber(*n) == 0) {
		printf("Dividing by Zero is not allowed.\n");
		return NULL;
	}
	zeroRemov(m);
	zeroRemov(n);

	int k = m->dec > n->dec ? m->dec : n->dec;
	int i = 0;

	while(i < k) {
		if(m->dec > 0)
			m->dec--;
		else
			addDigit(m, '0');
		if(n->dec > 0)
			n->dec--;
		else
			addDigit(n, '0');
		i++;
	}
	i = 9;
	number *c, *d, *ans, *q, *pro;
	c = (number *)malloc(sizeof(number));
	d = (number *)malloc(sizeof(number));
	ans = (number *)malloc(sizeof(number));
	pro = (number *)malloc(sizeof(number));
	q = (number *)malloc(sizeof(number));

	initNumber(ans);
	initNumber(c);
	initNumber(q);
	initNumber(d);
	if(m->sign == n->sign) {
		q->sign = PLUS;
		m->sign = n->sign = PLUS;
	}
	else {
		q->sign = MINUS;
		m->sign = n->sign = PLUS;
	}
	node *p = m->head;
	char ch = p->num + '0';
	addDigit(d, ch);
	while(q->dec < SCALE){
		while(i >= 0){
			appendleft(c, i);
			pro = mult(n, c);
			ans = sub(d, pro);
			if(ans->sign != MINUS) {
				addDigit(q, i + '0');
				node *tmp = c->head;
				free(tmp);
				c->head = c->tail = NULL;
				break;
			}
			else{
				node *tmp = c->head;
				free(tmp);
				c->head = c->tail = NULL;
				i--;
			}
		}
		d = ans;
		if(p->next != NULL) {
			p = p->next;
			ch = p->num + '0';
			addDigit(d, ch);
		}
		else{
			q->dec++;
			addDigit(d, '0');
		}
		i = 9;
		node *tmp = c->head;
		free(tmp);
		c->head = c->tail = NULL;
	}
	q->dec--;
	return q;
}

//Modulus Operation
number *modulus(number *a, number *b) {
	if(zeroNumber(*b) == 0) {
		printf("ERROR : Modulo by Zero is not allowed.\n");
		return NULL;
	}
	int tempsign;
	if(a->sign == MINUS) {
		tempsign = MINUS;
		a->sign = b->sign = PLUS;
	}
	else {
		tempsign = PLUS;
		a->sign = b->sign = PLUS;
	}
	decimalEqual(a, b);
	int a_dec = a->dec;
	number *ans = (number *)malloc(sizeof(number));
	number *temp = (number *)malloc(sizeof(number));
	initNumber(ans);
	initNumber(temp);
	temp = division(a, b);
	if(temp->dec != 0) {
		int pos = length(*temp) - 1;
		while(temp->dec != 0) {
			remov(temp, pos);
			temp->dec--;
			pos--;
		}
	}
	temp = mult(temp, b);
	ans = sub(a, temp);
	ans->sign = tempsign;
	ans->dec = a_dec;
	return ans;
}


int precedence(char o) {
	switch(o) {
		case 'S':
		case 'C':
		case 'T':
			return -1;
			break;
		case ')' :
			return 0;
			break;
		case '+' :
		case '-' :
			return 1;
			break;
		case '*' :
		case '/' :
			return 2;
			break;
		case '%' :
			return 3;
			break;
		case '(' :
			return 4;
			break;
		default :
			return 5;
			break;
	}
	return 5;
}

typedef struct token{
	int type;
	number *num;
	char op;
}token;

enum state { NUMBER, DOT, OP, STOP, ERROR, SPACE };

token gettoken(char *expr, int *reset) {
	static int i = 0;
	number *a = (number *)malloc(sizeof(number));
	initNumber(a);
	char currchar;
	static enum state currstate = SPACE;
	enum state nextstate;
	token t;
	if(*reset == 1) {
		currstate = SPACE;
		*reset = 0;
		i = 0;
	}
	while(1) {
		currchar = expr[i];
		switch(currstate) {
			case NUMBER:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						nextstate = NUMBER;
						addDigit(a, currchar);
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
				}
				break;

			case DOT:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						addDigit(a, currchar);
						a->dec++;
						nextstate = DOT;
						currstate = nextstate;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						i++;
						currstate = nextstate;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERAND;
						t.num = a;
						currstate = nextstate;
						return t;
						break;
				}
				break;

			case OP:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						t.type = OPERATOR;
						t.op = expr[i - 1];
						nextstate = NUMBER;
						currstate = nextstate;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						nextstate = STOP;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
					case ' ':
						nextstate = SPACE;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;

					case '.':
						nextstate = DOT;
						t.type = OPERATOR;
						t.op = expr[i -1];
						currstate = nextstate;
						i++;
						return t;
						break;

					default:
						nextstate = ERROR;
						t.type = OPERATOR;
						t.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
				}
				break;
			case STOP:
				t.type = END;
				return t;
				break;
			case ERROR:
				t.type = ERR;
				return t;
				break;
			case SPACE:
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						addDigit(a, currchar);
						nextstate = NUMBER;
						i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(' : case ')' :
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = STOP;
						break;
					case ' ':
						nextstate = SPACE;
						i++;
						break;

					case '.':
						nextstate = DOT;
						i++;
						break;

					default:
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
		}
	}
}


number *infix(char *exp) {
	token t;
	char curr_op, prev_op, ch;
	int cnt_ob = 0, cnt_cb = 0, reset  = 1, curr, prev;
	number *a, *b, *z;
	a = (number *)malloc(sizeof(number));
	b = (number *)malloc(sizeof(number));
	z = (number *)malloc(sizeof(number));
	initNumber(a);
	initNumber(b);
	initNumber(z);
	istack is;
	cstack cs;
	iinit(&is);
	cinit(&cs);
	prev = ERR;
	while(1) {
		t = gettoken(exp, &reset);
		curr = t.type;
		if(curr == prev && prev == OPERAND) {
			return NULL;
		}
		if(t.type == OPERAND){
			ipush(&is, t.num);
		}
		else if(t.type == OPERATOR){
			curr_op = t.op;
			if(curr_op == '(')
				cnt_ob++;
			if(curr_op == ')')
				cnt_cb++;
			if(cnt_cb > cnt_ob)
				return NULL;
			if(!cisempty(&cs)) {
				prev_op = ctop(&cs);

				while(precedence(prev_op) >= precedence(curr_op)) {
					prev_op = cpop(&cs);
					if(!iisempty(&is)) {
						a = ipop(&is);
					}
					else{
						cpush(&cs, prev_op);
						break;
					}
					if(!iisempty(&is)) {
						b = ipop(&is);
					}
					else {
						cpush(&cs, prev_op);
						ipush(&is, a);
						break;
					}
					if((prev_op == '/' || prev_op == '%') && a == 0) {
                                                printf("Mathematical error\n");
                                        	return NULL;
                                        }
					switch (prev_op) {
						case '+' :
							//z = b + a;
							z = add(a, b);
							ipush(&is, z);
							break;
						case '-' :
							//z = b - a;
							z = sub(b, a);
							ipush(&is, z);
							break;
						case '*' :
							//z = b * a;
							z = mult(b, a);
							ipush(&is, z);
							break;
						case '/' :
							//z = b / a;
							z = division(b, a);
							ipush(&is, z);
							break;
						case '%' :
							//z = b % a;
							z = modulus(b, a);
							ipush(&is, z);
							break;

						case '(' :
							cpush(&cs, prev_op);
							ipush(&is, b);
							ipush(&is, a);
							break;
						default :
							return NULL;
					}
					if (prev_op == '(')
						break;
					if(!cisempty(&cs))
						prev_op = ctop(&cs);
					else
						break;
				}
			}
			cpush(&cs, t.op);
			if(curr_op == ')') {
				ch = cpop(&cs);
				cnt_cb--;
				ch = cpop(&cs);
				cnt_ob--;
			}
		}
		else if(t.type == END) {
			if(cnt_ob == cnt_cb) {
				while(!cisempty(&cs)) {
					if(!iisempty(&is)) {
						a = ipop(&is);
					}
					else {
						printf("Less operands\n");
                        return NULL;
					}
					if(!iisempty(&is)) {
						b = ipop(&is);
					}
					else {
						printf("Less operands\n");
                        return NULL;
					}
					ch = cpop(&cs);
					if((ch == '/' || ch == '%') && a == 0) {
						printf("Mathematical error\n");
						return NULL;
					}
					switch(ch) {
						case '+' :
							z = add(b, a);
							ipush(&is, z);
							break;
						case '-' :
							z = sub(b, a);
							ipush(&is, z);
							break;
						case '*' :
							z = mult(b, a);
							ipush(&is, z);
							break;
						case '/' :
							z = division(b, a);
							ipush(&is, z);
							break;
						case '%' :
							z = modulus(b, a);
							ipush(&is, z);
							break;
						default :
							return NULL;
					}
				}
			}
			else {
				printf("Error in Expression\n");
				return NULL;
			}
			if(!iisempty(&is)){
				z = ipop(&is);
				if(iisempty(&is))
					return z;
				else {
                    printf("Less Operators\n");
                    return NULL;
                }
			}
			else {
				printf("Less Operands\n");
				return NULL;
			}
		}
		else if(t.type == ERR) {
			return NULL;
		}
		prev = curr;
	}
}

void modify(char *line) {
	int i;
	for(i = 0; i < strlen(line); i++) {
		if(line[i] == '(') {
			int j = 1;
			while(1) {
				if(line[i + j] == ' ') {
					j++;
				}
				else {
					break;
				}
			}
			if((line[i + j] == '-') || (line[i + j] == '+')) {
				int temp = strlen(line);
				while(temp >= i + j) {
					line[temp + 1] = line[temp];
					temp--;
				}
				line[i + j] = '0';
			}
		}
	}
}
int readline(char *line, int len) {
	int i;
	char ch;
	i = 0;
	while(i < len - 1) {
		ch = getchar();
		if(ch == '\n') {
			line[i++] = '\0';
			return i - 1;
		}
		else
			line[i++] = ch;
	}
	line[len - 1] = '\0';
	return len - 1;
}

int main() {
	number *r;
	char line[2048];
	while(readline(line, 2048)) {
		modify(line);
		r = infix(line);
		if(r != NULL)
			printNumber(*r);
		else
			printf("Error in expression\n");
	}
	return 0;
}