#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strCapacity 20
/*存表达式项的链表节点*/
typedef struct LinkNode {
	int		flag;				//用来指示是不是未知数项,0为普通项，1为未知数项
	int		priority;			//操作符优先级
	char	*str;				//表达式单个项
	struct LinkNode *next;
}List;

/*-----------------------------链表和栈的操作----------------------------------*/
//链表头节点初始化
void StackInit(List *stack) {
	stack = (List *)malloc(sizeof(List));
	stack->priority = 0;
	stack->flag = 0;
	stack->str = NULL;
	stack->next = NULL;
}

//进栈
void Push(List *stack, List *node) {
	if (stack == NULL) {
		return;
	}
	node->next = stack->next;
	stack->next = node;
}

//出栈
void Pop(List *stack, List **node) {
	if (stack->next == NULL) {
		return;
	}
	List *tmp = stack->next;
	stack->next = stack->next->next;
	tmp->next = NULL;
	*node = tmp;
}

//删除栈顶元素
void Pop_delete(List *stack) {
	if (stack == NULL) {
		return;
	}
	stack->next = stack->next->next;
}

//尾插法
void node_Insert(List *stack, List *node) {
	if (stack == NULL) {
		return;
	}
	while (stack->next != NULL) {
		stack = stack->next;
	}
	stack->next = node;
	
}

//得到栈顶元素的优先级
int get_stack_top_priority(List *stack) {
	if (stack == NULL) {
		return -1;
	}
	return stack->next->priority;
}
/*-----------------------------------------------------------------------*/


/*------------------------------功能函数----------------------------------*/

//节点的内存拷贝函数，注意，结构体内有指针，所以需要深拷贝
void nodememCopy(List *src, List **des) {
	List *local = (List *)malloc(sizeof(List));
	local->flag = src->flag;
	local->next = NULL;
	local->priority = src->priority;
	local->str = (char *)malloc(strCapacity * sizeof(char));		//深拷贝
	memset(local->str, '\0', strCapacity);
	strcpy(local->str, src->str);
	*des = local;
}

//去掉字符串结尾的那个x
void remove_X(char *des) {
	char *tmp = des;
	while (*tmp != 'x') {
		tmp++;
	}
	*tmp = '\0';
}

//申请内存
void malloc_calcMem(char **strTmp1, char **strTmp2) {
	char *tmp1 = (char *)malloc(strCapacity * sizeof(char));
	memset(tmp1, '\0', strCapacity);
	char *tmp2 = (char *)malloc(strCapacity * sizeof(char));
	memset(tmp2, '\0', strCapacity);
	*strTmp1 = tmp1;
	*strTmp2 = tmp2;
}

//把普通数字打包进节点然后入栈
void dataPack_and_push(double tmpNumber, List *calcStack) {
	List *calcedNode = (List *)malloc(sizeof(List));	//将计算完成的数字打包
	calcedNode->str = (char *)malloc(sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;
	//sprintf(calcedNode->str, "%lf", tmpNumber);
	_gcvt(tmpNumber, 2, calcedNode->str);
	char *addX = calcedNode->str;
	while (*addX != '\0') {
		addX++;
	}
	*addX = 'x';
	Push(calcStack, calcedNode);			//入栈
}

//判断这是复合的表达式还是普通表达式，例：1+2x，这种是不能直接处理的，要拆开看
int complex_expression(List *node) {
	char *tmp = node->str;
	while (*tmp != '\0') {
		if (*tmp != '+' && *tmp != '-' && *tmp != '*' && *tmp != '/') {
			tmp++;
		}
		else {
			return 1;		//1是复杂表达式
		}
	}
	return 0;		//0是普通数字
}

//判断这个复合表达式的操作符是+还是-,是+返回1，是-返回0，都没有就返回-1发生错误
int judge_plus(List *node) {
	char *ptr = node->str;
	while (*ptr != '\0') {
		if (*ptr == '+' || *ptr == '-') {
			if (*ptr == '+') {
				return 1;
			}
			else {
				return 0;
			}
		}
		ptr++;
	}
	return -1;
}

//判断里面的数字是不是负数,是负数返回1，不是返回0
int negative(List *node) {
	char *ptr = node->str;
	if (*ptr == '-') {
		return 1;
	}
	else {
		return 0;
	}
}


//判断两个字符串是不是相等的,为0则相等，不为0则不相等
int equalStr(List *node1, List *node2) {
	int ret = strcmp(node1->str, node2->str);
	return ret;
}

//把复杂表达式的系数和普通数字分离出来
void getNumber(List *node, char *nodeCoefficient, char *nodeNumber) {
	char *nodeStrPtr = node->str;
	while (*nodeStrPtr != 'x') {
		*nodeCoefficient = *nodeStrPtr;
		nodeCoefficient++;
		nodeStrPtr++;
	}
	nodeStrPtr++;
	if (*nodeStrPtr == '+') {
		nodeStrPtr++;
	}
	while (*nodeStrPtr != '\0') {
		*nodeNumber = *nodeStrPtr;
		nodeNumber++;
		nodeStrPtr++;
	}
}

//普通的两个数字的操作
void simpleNumber_process(char *str1,char *str2, char *caledStr, char ch) {
	double tmpNumber1 = atof(str1);		//拿到要运算的两个操作数
	double tmpNumber2 = atof(str2);
	double tmpNumber = 0;							//用来存运算过后的数字
	switch (ch)
	{
	case '+':
		tmpNumber = tmpNumber1 + tmpNumber2;
		break;
	case '-':
		tmpNumber = tmpNumber1 - tmpNumber2;
		break;
	case '*':
		tmpNumber = tmpNumber1 * tmpNumber2;
		break;
	case '/':
		tmpNumber = tmpNumber1 / tmpNumber2;
		break;
	}
	//sprintf(caledStr, "%lf", tmpNumber);  //这个函数在存负数时会发生问题，弃用
	_gcvt(tmpNumber, 5, caledStr);		//把浮点数转换为字符串
	while (*caledStr != '\0') {			//这一段的作用是，当计算出来是个位整数时，写入caledStr会在数字后面多一个'.' 这里就要去掉这个'.'
		if (*caledStr == '.') {
			caledStr++;
			if (*caledStr == '\0') {
				caledStr--;
				*caledStr = '\0';
			}
			break;
		}
		caledStr++;
	}
}

//这个函数的作用是把两个普通操作数组合为一个字符串ax+b，例如1 + 2x, 1 - 2x, -1 - 2x,仅仅限于普通未知数
void simple_combine_singleX(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	List *calcedNode = (List *)malloc(sizeof(List));
	calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;

	char *strTmp1 = NULL;
	char *strTmp2 = NULL;
	malloc_calcMem(&strTmp1, &strTmp2);
	strcpy(strTmp1, nodeleft->str);		//把未知数项存到strTmp这个字符串里
	strcpy(strTmp2, nodeRight->str);
	char *strTmp1Ptr = strTmp1;
	char *strTmp2Ptr = strTmp2;
	char *cpmplexStr = (char *)malloc(strCapacity * sizeof(char));
	memset(cpmplexStr, '\0', strCapacity);
	char *complexStrPointer = cpmplexStr;
	if (nodeleft->flag == 1) {			//未知数在前面
		if (negative(nodeRight)) {		//常数项为负数
			while (*strTmp1Ptr != '\0') {				//将两个字符串连成一起
				*complexStrPointer = *strTmp1Ptr;
				complexStrPointer++;
				strTmp1Ptr++;
			}

			if (ch == '+') {
				*complexStrPointer = '-';
				complexStrPointer++;
			}
			else {
				*complexStrPointer = '+';
				complexStrPointer++;
			}

			while (*strTmp2Ptr != '\0') {
				*complexStrPointer = *strTmp2Ptr;
				complexStrPointer++;
				strTmp2Ptr++;
			}
		}
		else {			//常数项为正数
			while (*strTmp1Ptr != '\0') {				//将两个字符串连成一起
				*complexStrPointer = *strTmp1Ptr;
				complexStrPointer++;
				strTmp1Ptr++;
			}
			*complexStrPointer = ch;
			complexStrPointer++;
			while (*strTmp2Ptr != '\0') {
				*complexStrPointer = *strTmp2Ptr;
				complexStrPointer++;
				strTmp2Ptr++;
			}
		}
	}
	else {								//未知数项在后面,有几种情形：(1+2x)  (1 + -2x) (-1+2x) (-1 + -2x) (1-2x) (1 - -2x) (-1 - 2x) (-1 - -2x)
		if (ch == '+') {
			while (*strTmp2Ptr != '\0') {
				*complexStrPointer = *strTmp2Ptr;
				complexStrPointer++;
				strTmp2Ptr++;
			}
			if (!negative(nodeleft)) {		//常数项是正数，将它放到后面时，要在它前面加一个+号
				*complexStrPointer = '+';
				complexStrPointer++;
			}
			while (*strTmp1Ptr != '\0') {				//将两个字符串连成一起
				*complexStrPointer = *strTmp1Ptr;
				complexStrPointer++;
				strTmp1Ptr++;
			}
		}
		else {		//操作数是-的情况
			if (negative(nodeRight)) {   //系数如果是负数  (1 - -2x)  (-1 - -2x)
				strTmp2Ptr++;     //去掉前面那个负号，因为负负得正
				while (*strTmp2Ptr != '\0') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				}
				if (!negative(nodeleft)) {
					*complexStrPointer = '+';
					complexStrPointer++;
				}
				while (*strTmp1Ptr != '\0') {				//将两个字符串连成一起
					*complexStrPointer = *strTmp1Ptr;
					complexStrPointer++;
					strTmp1Ptr++;
				}
			}
			else {     //系数如果是正数  (1-2x)  (-1 - 2x)
				*complexStrPointer = '-';
				while (*strTmp2Ptr != '\0') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				}
				if (!negative(nodeleft)) {
					*complexStrPointer = '+';
					complexStrPointer++;
				}
				while (*strTmp1Ptr != '\0') {				//将两个字符串连成一起
					*complexStrPointer = *strTmp1Ptr;
					complexStrPointer++;
					strTmp1Ptr++;
				}
			}
		}
	}
	calcedNode->str = cpmplexStr;
	Push(calcStack, calcedNode);			//入栈
	free(strTmp1);
	free(strTmp2);
}

//这个函数的作用是把两个操作数组合为一个字符串(+-的情况)，其中一个是复合型字符串，例如2x+1,-2x+1,另一个是普通数字，例如1,2,-3
//合起来大概就是这种形式：(2x+1) - 1 , (2x-3) + -4
void complex_combine_singleX1(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	List *calcedNode = (List *)malloc(sizeof(List));
	calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;

	char *strTmp1 = NULL;
	char *strTmp2 = NULL;
	malloc_calcMem(&strTmp1, &strTmp2);
	char *strTmp1Ptr = strTmp1;
	char *strTmp2Ptr = strTmp2;
	strcpy(strTmp1, nodeleft->str);		//把未知数项存到strTmp这个字符串里
	strcpy(strTmp2, nodeRight->str);
	char *cpmplexStr = (char *)malloc(strCapacity * sizeof(char));
	memset(cpmplexStr, '\0', strCapacity);
	char *complexStrPointer = cpmplexStr;
	if (complex_expression(nodeleft)) {		//左侧是复杂表达式
		char *str1 = NULL;
		char *str2 = NULL;
		malloc_calcMem(&str1, &str2);
		char *str1Ptr = str1;
		char *str2Ptr = str2;

		while (*strTmp1Ptr != '+' && *strTmp1Ptr != '-') {
			*complexStrPointer = *strTmp1Ptr;
			complexStrPointer++;
			strTmp1Ptr++;
		} //现在temp指针指向操作符号那里,complexStrPointer指向下一个要填充的字符那里
		if (judge_plus(nodeleft)) {
			strTmp1Ptr++;
		}
		while (*strTmp1Ptr != '\0') {
			*str1Ptr = *strTmp1Ptr;
			str1Ptr++;
			strTmp1Ptr++;
		}
		while (*strTmp2Ptr != '\0') {		//在这里,strTemp2指向的是普通数字字符串
			*str2Ptr = *strTmp2Ptr;
			str2Ptr++;
			strTmp2Ptr++;
		}
		char *caledStr = (char *)malloc(strCapacity * sizeof(char));
		memset(caledStr, '\0', strCapacity);
		char *caledStrPtr = caledStr;
		simpleNumber_process(str1, str2, caledStr, ch);			//把(2x+1) - -1 中的1 - -1计算出来，放在caledStr里面
		free(str1);
		free(str2);
		if (*caledStrPtr == '-') {
			while (*caledStrPtr != '\0') {
				*complexStrPointer = *caledStrPtr;
				complexStrPointer++;
				caledStrPtr++;
			}
		}
		else {
			*complexStrPointer = '+';
			complexStrPointer++;
			while (*caledStrPtr != '\0') {
				*complexStrPointer = *caledStrPtr;
				complexStrPointer++;
				caledStrPtr++;
			}
		}
		free(caledStr);		//记得把这块内存释放掉
	}
	else {		//右侧是复杂表达式
		char *str1 = NULL;
		char *str2 = NULL;
		malloc_calcMem(&str1, &str2);
		char *str1Ptr = str1;
		char *str2Ptr = str2;

		if (ch == '+') {
			while (*strTmp2Ptr != '+' && *strTmp2Ptr != '-') {
				*complexStrPointer = *strTmp2Ptr;
				complexStrPointer++;
				strTmp2Ptr++;
			} //现在temp指针指向操作符号那里,complexStrPointer指向下一个要填充的字符那里
		}
		else {		//ch == '-'
			if (*strTmp2Ptr == '-') {
				strTmp2Ptr++;
				while (*strTmp2Ptr != '+' && *strTmp2Ptr != '-') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				} //现在temp指针指向操作符号那里,complexStrPointer指向下一个要填充的字符那里
			}
			else {
				*complexStrPointer = '-';
				while (*strTmp2Ptr != '+' && *strTmp2Ptr != '-') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				} //现在temp指针指向操作符号那里,complexStrPointer指向下一个要填充的字符那里
			}
		}

		while (*strTmp1Ptr != '\0') {		//这是把左边那个普通数字取出来
			*str1Ptr = *strTmp1Ptr;
			str1Ptr++;
			strTmp1Ptr++;
		}
		if (judge_plus(nodeRight)) {
			strTmp2Ptr++;
		}
		while (*strTmp2Ptr != '\0') {
			*str2 = *strTmp2Ptr;
			str2++;
			strTmp2Ptr++;
		}		
		char *caledStr = (char *)malloc(strCapacity * sizeof(char));
		memset(caledStr, '\0', strCapacity);
		char *caledStrPtr = caledStr;
		simpleNumber_process(str1, str2, caledStr, ch);			//把(2x+1) - -1 中的1 - -1计算出来，放在caledStr里面
		if (*caledStrPtr == '-') {
			while (*caledStrPtr != '\0') {
				*complexStrPointer = *caledStrPtr;
				complexStrPointer++;
				caledStrPtr++;
			}
		}
		else {
			*complexStrPointer = '+';
			complexStrPointer++;
			while (*caledStrPtr != '\0') {
				*complexStrPointer = *caledStrPtr;
				complexStrPointer++;
				caledStrPtr++;
			}
		}
		free(caledStr);		//记得把这块内存释放掉
	}
	calcedNode->str = cpmplexStr;
	Push(calcStack, calcedNode);			//入栈
	free(strTmp1);
	free(strTmp2);
}

//这个函数的作用是把两个操作数组合为一个字符串（*/的情况），其中一个是复合型字符串，例如2x+1,-2x+1,另一个是普通数字，例如1,2,-3
//合起来大概就是这种形式：(2x+1) * 1 , (2x-3) / (-4)
void complex_combine_singleX2(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	char *node1strTmp1 = NULL;				//用来存2x+1的系数2和常数1的字符串及其游标指针
	char *node1strTmp2 = NULL;
	malloc_calcMem(&node1strTmp1, &node1strTmp2);
	char *node1StrPtr1 = node1strTmp1;
	char *node1StrPtr2 = node1strTmp2;

	char *node2strTmp1 = NULL;				//用来存(2x+1)*(-5)中5或者-5的字符串及其游标指针
	char *node2strTmp2 = NULL;
	malloc_calcMem(&node2strTmp1, &node2strTmp2);
	char *node2StrPtr1 = node2strTmp1;
	char *node2StrPtr2 = node2strTmp2;

	char *node1Ptr = nodeleft->str;		//(2x+1)和(-5)的原始字符串指针
	char *node2Ptr = nodeRight->str;

	while (*node1Ptr != 'x') {
		*node1StrPtr1 = *node1Ptr;
		node1StrPtr1++;
		node1Ptr++;
	}
	node1Ptr++;		//跳过x，现在指向复杂表达式的操作符上
	char opt = *node1Ptr;
	node1Ptr++;
	while (*node1Ptr != '\0') {
		*node1StrPtr2 = *node1Ptr;
		node1StrPtr2++;
		node1Ptr++;
	}

	strcpy(node2StrPtr1, node2Ptr);

	char symbol = *node2Ptr;
	if (symbol == '-') {
		node2Ptr++;
		strcpy(node2StrPtr2, node2Ptr);
	}
	else {
		symbol = '+';
		strcpy(node2StrPtr2, node2Ptr);
	}
	//----------------这里取系数和常数字符串的工作已经做完,接下来该计算了------
	char *coefficient = (char *)malloc(strCapacity * sizeof(char));
	memset(coefficient, '\0', strCapacity);
	char *coefficientPtr = coefficient;
	char *normalNumber = (char *)malloc(strCapacity * sizeof(char));
	memset(normalNumber, '\0', strCapacity);
	char *normalNumberPtr = normalNumber;

	simpleNumber_process(node1strTmp1, node2strTmp1, coefficient, ch);
	simpleNumber_process(node1strTmp2, node2strTmp2, normalNumber, ch);
	free(node1strTmp1);
	free(node1strTmp2);
	free(node2strTmp1);
	free(node2strTmp2);

	List *calcedNode = (List *)malloc(sizeof(List));
	calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;

	char *calcedNodeStrPtr = calcedNode->str;
	while (*coefficientPtr != '\0') {
		*calcedNodeStrPtr = *coefficientPtr;
		calcedNodeStrPtr++;
		coefficientPtr++;
	}
	*calcedNodeStrPtr = 'x';
	calcedNodeStrPtr++;
	if ((opt == '+' && symbol == '+') || (opt == '-' && symbol == '-')) {
		*calcedNodeStrPtr = '+';
		calcedNodeStrPtr++;
	}
	else {
		*calcedNodeStrPtr = '-';
		calcedNodeStrPtr++;
	}
	while (*normalNumberPtr != '\0') {
		*calcedNodeStrPtr = *normalNumberPtr;
		calcedNodeStrPtr++;
		normalNumberPtr++;
	}
	Push(calcStack, calcedNode);
}

//这个函数的作用是把两个操作数组合为一个字符串,两个都是含x的未知项，一个是复合项，一个是普通x项。大概就是(2x-1) + 2x,2x - (2x-1)
void complex_combine_doubleX(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	char *node1Coefficient = NULL;
	char *node1Number = NULL;
	malloc_calcMem(&node1Coefficient, &node1Number);
	getNumber(nodeleft, node1Coefficient, node1Number);

	char *node2Coefficient = (char *)malloc(strCapacity * sizeof(char));
	memset(node2Coefficient, '\0', strCapacity);
	char *node2CoefficientPtr = node2Coefficient;
	char *node2Ptr = nodeRight->str;
	while (*node2Ptr != 'x') {
		*node2CoefficientPtr = *node2Ptr;
		node2CoefficientPtr++;
		node2Ptr++;
	}

	char *coefficient = (char *)malloc(strCapacity * sizeof(char));
	memset(coefficient, '\0', strCapacity);

	simpleNumber_process(node1Coefficient, node2Coefficient, coefficient, ch);

	List *calcedNode = (List *)malloc(sizeof(List));
	calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;

	char *ptr1 = coefficient;
	char *ptr2 = node1Number;

	char *calcedNodeStrPtr = calcedNode->str;
	while (*ptr1 != '\0') {
		*calcedNodeStrPtr = *ptr1;
		calcedNodeStrPtr++;
		ptr1++;
	}
	*calcedNodeStrPtr = 'x';
	calcedNodeStrPtr++;
	if (*ptr2 != '-') {
		*calcedNodeStrPtr = '+';
		calcedNodeStrPtr++;
	}
	while (*ptr2 != '\0') {
		*calcedNodeStrPtr = *ptr2;
		calcedNodeStrPtr++;
		ptr2++;
	}
	Push(calcStack, calcedNode);

	free(node1Coefficient);
	free(node1Number);
	free(node2Coefficient);
	free(coefficient);
}

//这个函数的作用是把两个操作数组合为一个字符串,两个都是含x的未知项，一个是复合项，一个是普通x项。大概就是(2x-1) + (2x+5),(5x-3) - (2x-1)
void double_complex_combine(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	char *node1Coefficient = NULL;
	char *node1Number = NULL;
	malloc_calcMem(&node1Coefficient, &node1Number);
	getNumber(nodeleft, node1Coefficient, node1Number);

	char *node2Coefficient = NULL;
	char *node2Number = NULL;
	malloc_calcMem(&node2Coefficient, &node2Number);
	getNumber(nodeRight, node2Coefficient, node2Number);

	char *coefficient = NULL;
	char *number = NULL;
	malloc_calcMem(&coefficient, &number);

	simpleNumber_process(node1Coefficient, node2Coefficient, coefficient, ch);
	simpleNumber_process(node1Number, node2Number, number, ch);
	free(node1Coefficient);
	free(node1Number);
	free(node2Coefficient);
	free(node2Number);

	List *calcedNode = (List *)malloc(sizeof(List));
	calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
	memset(calcedNode->str, '\0', strCapacity);
	calcedNode->flag = 1;
	calcedNode->next = NULL;
	calcedNode->priority = 0;

	char *ptr1 = coefficient;
	char *ptr2 = number;

	char *calcedNodeStrPtr = calcedNode->str;
	while (*ptr1 != '\0') {
		*calcedNodeStrPtr = *ptr1;
		calcedNodeStrPtr++;
		ptr1++;
	}
	*calcedNodeStrPtr = 'x';
	calcedNodeStrPtr++;
	if (*ptr2 != '-') {
		*calcedNodeStrPtr = '+';
		calcedNodeStrPtr++;
	}
	while (*ptr2 != '\0') {
		*calcedNodeStrPtr = *ptr2;
		calcedNodeStrPtr++;
		ptr2++;
	}

	Push(calcStack, calcedNode);
	free(coefficient);
	free(number);
}

//中缀表达式转后缀表达式
List *infix_TO_suffix(char *expression) {
	if (expression == NULL) {
		return NULL;
	}

	List *element = (List *)malloc(sizeof(List));
	element->flag = 0;
	element->next = NULL;
	element->priority = 0;
	element->str = NULL;

	char *temp = expression;		//遍历指针

	List *stack = NULL;				//临时的那个存操作符 ()+-*/ 的栈
	stack = (List *)malloc(sizeof(List));
	stack->priority = 0;
	stack->next = NULL;
	stack->str = NULL;

	List *first_Node = (List *)malloc(sizeof(List));		//第一个优先级为0的节点
	first_Node->flag = 0;
	first_Node->next = NULL;
	first_Node->str = NULL;
	first_Node->priority = 0;
	Push(stack, first_Node);		//优先级为0的这个节点入栈

	while (*temp != '\0') {			//循环遍历字符串里的每个字符
		if (*temp == 'x') {			//这种情况是单个项为x的情形
			char *singleX = (char *)malloc(strCapacity * sizeof(char));
			memset(singleX, '\0', strCapacity);
			char *singleXPtr = singleX;
			*singleXPtr++ = '1';		//在这里主要是为了计算方便起见，在系数为1的x前面都加上一个1
			*singleXPtr = *temp;
			List *node = (List *)malloc(sizeof(List));
			node->str = singleX;
			node->flag = 1;
			node->priority = 0;
			node->next = NULL;
			node_Insert(element, node);		//普通数，输出
			temp++;
		}
		else if (*temp == '+' || *temp == '-' || *temp == '*' || *temp == '/' || *temp == '(' || *temp == ')') {		//这种为普通的+-*/()
			int out_priority;								//+-*/在栈外的优先级

			char *s = (char *)malloc(strCapacity * sizeof(char));
			memset(s, '\0', strCapacity);
			*s = *temp;
			List *node = (List *)malloc(sizeof(List));
			node->str = s;
			node->flag = 0;
			node->next = NULL;
			switch (*temp)
			{
			case '+':
				node->priority = 3;
				out_priority = 2;
				break;
			case '-':
				node->priority = 3;
				out_priority = 2;
				break;
			case '*':
				node->priority = 5;
				out_priority = 4;
				break;
			case '/':
				node->priority = 5;
				out_priority = 4;
				break;
			case '(':
				node->priority = 1;
				out_priority = 6;
				break;
			case ')':
				node->priority = 6;
				out_priority = 1;
				break;
			}

			int in_priority = get_stack_top_priority(stack);	//取得临时栈顶的优先级
			if (out_priority > in_priority) {					//一、栈外>栈内，进栈
				Push(stack, node);		//进栈
			}
			else if (out_priority < in_priority) {				//二、栈外<栈内,退栈并输出,并且把优先级高的那个操作符进栈
				while (out_priority < get_stack_top_priority(stack)) {		//如果栈内元素优先级一直比外面那个元素大，循环将它退栈输出
					List *tmpnode = NULL;
					Pop(stack, &tmpnode);		//退栈
					tmpnode->next = NULL;
					node_Insert(element, tmpnode);		//输出
				}

				if (*(node->str) != ')') {		//如果不是‘)’符号，就进栈，否指')'没有必要进栈
					Push(stack, node);
				}
				else {							//如果是')'符号，则它不用进栈，而且判断栈顶是不是'('符号，若是，它俩配成一对，没用，退栈，丢掉'('
					if (*(stack->next->str) == '(') {
						Pop_delete(stack);
					}
				}
			}
			else {												//三、栈外=栈内，直接退栈，只有（）才满足，
				Pop_delete(stack);
			}
			temp++;
		}
		else if (*temp >= 48 && *temp <= 57) {
			char *s = (char *)malloc(strCapacity * sizeof(char));
			memset(s, '\0', strCapacity);
			List *node = (List *)malloc(sizeof(List));

			char *tmp = s;				//这是存操作数的游标
			while ((*temp >= 48 && *temp <= 57) || *temp == 'x') {
				*tmp = *temp;
				temp++;
				tmp++;
			}
			tmp--;
			if (*tmp == 'x') {
				node->flag = 1;
			}
			else {
				node->flag = 0;
			}
			node->str = s;
			node->priority = 0;
			node->next = NULL;
			node_Insert(element, node);		//普通数，输出
		}
		else {			//存在非数学表达式符号
			return NULL;	//表达式错误
		}
	}

	while (stack->next->str != NULL) {		//把栈内剩余的操作符输出
		List *tmpnode = NULL;
		Pop(stack, &tmpnode);
		tmpnode->next = NULL;
		if (*(tmpnode->str) != '(' && *(tmpnode->str) != ')') {
			node_Insert(element, tmpnode);
		}
	}

	return element;
}

//这个函数是用来将后缀表达式转换为ax+b形式的
int process(List *suffix_expression, List *calcStack) {
	List *pointer = suffix_expression->next;  //后缀表达式链的一个游标
	while (pointer != NULL) {
		if (*(pointer->str) != '+' && *(pointer->str) != '-' && *(pointer->str) != '*' && *(pointer->str) != '/') {		//这是普通操作数的情况，将他们入栈
			List *node = NULL;
			nodememCopy(pointer, &node);
			Push(calcStack, node);
			pointer = pointer->next;
		}
		else {			//这种就是遇到运算符了，需要先从栈中弹出两个数进行计算
			List *calcNode1 = NULL;
			List *calcNode2 = NULL;
			Pop(calcStack, &calcNode2);			//这里注意，calcNode2是在操作符后面那个数，而它在栈顶，所以说要先取它，再取calcNode1
			Pop(calcStack, &calcNode1);
			char ch = *(pointer->str);									//这是操作符
			if (calcNode1->flag == 1 || calcNode2->flag == 1) {			//含有x的未知数项
				if (calcNode1->flag == 1 && calcNode2->flag == 1) {		//相加的两个都是含有x的未知数项
					int ret1 = complex_expression(calcNode1);
					int ret2 = complex_expression(calcNode2);
					if (ret1 == 1 && ret2 == 1) {			//左边和右边都是复杂表达式，只能实现+-/，而且/的情况下，两个未知项必须一样
						if (ch == '+' || ch == '-') {
							double_complex_combine(calcNode1, calcNode2, calcStack, ch);
						}
						else if (ch == '/') {
							if (equalStr(calcNode1, calcNode2) == 0) {
								int tmpNumber = 1;
								dataPack_and_push(tmpNumber, calcStack);
							}
							else {
								return -1;
							}
						}
						else {     //这种是(1+2x)*(1+2x)这种情形，会
							return -1;
						}
					}
					else if ((ret1 == 1 && ret2 == 0) || (ret1 == 0 && ret2 == 1)) {		//一个复杂，一个简单，只能+-
						if (ret1 == 1 && ret2 == 0) {		//左边复杂，右边简单
							if (ch == '*' || ch == '/') {
								return -1;
							}
							complex_combine_doubleX(calcNode1, calcNode2, calcStack, ch);
						}
						else {			//右边复杂，左边简单
							if (ch == '*' || ch == '/') {
								return -1;
							}
							complex_combine_doubleX(calcNode2, calcNode1, calcStack, ch);
						}
					}
					else {		//两个都是简单未知数项
						double tmpNumber = 0;								//用来存运算过后的数字
						char *strTmp1 = NULL;
						char *strTmp2 = NULL;
						malloc_calcMem(&strTmp1, &strTmp2);
						strcpy(strTmp1, calcNode1->str);		//把未知数项存到strTmp这个字符串里
						strcpy(strTmp2, calcNode2->str);
						remove_X(strTmp1);						//去掉了未知数项里面的x，剩下系数
						remove_X(strTmp2);
						double tmpNumber1 = atof(strTmp1);		//将其转为数字
						double tmpNumber2 = atof(strTmp2);
						switch (ch)								//根据操作符不同选择不同的操作方式
						{
						case '+':
							tmpNumber = tmpNumber1 + tmpNumber2;
							break;
						case '-':
							tmpNumber = tmpNumber1 - tmpNumber2;
							break;
						case '*':
							tmpNumber = tmpNumber1 * tmpNumber2;
							break;
						case '/':
							tmpNumber = tmpNumber1 / tmpNumber2;
							break;
						}
						if (ch == '+' || ch == '-') {		//+-的情况，这种情况还会带x参数，tmpNumber只是它的系数，除非tmpNumber=0可以丢掉这一项，否指还要在末尾加一个x
							if (tmpNumber != 0) {
								List *calcedNode = (List *)malloc(sizeof(List));	//将计算完成的数字打包
								calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
								memset(calcedNode->str, '\0', strCapacity);
								calcedNode->flag = 1;
								calcedNode->next = NULL;
								calcedNode->priority = 0;
								//sprintf(calcedNode->str, "%lf", tmpNumber);
								_gcvt(tmpNumber, 5, calcedNode->str);
								char *addX = calcedNode->str;
								while (*addX != '\0') {
									addX++;
								}
								*addX = 'x';
								Push(calcStack, calcedNode);			//入栈
							}
						}
						else {								//*和/的情况，/的这种情况x就消掉了，故只会剩余一个常数项，而*会产生二次方，不符合题意，报错
							if (ch == '/') {
								List *calcedNode = (List *)malloc(sizeof(List));	//将计算完成的数字打包
								calcedNode->str = (char *)malloc(sizeof(char));
								memset(calcedNode->str, '\0', strCapacity);
								calcedNode->flag = 0;
								calcedNode->next = NULL;
								calcedNode->priority = 0;
								//sprintf(calcedNode->str, "%lf", tmpNumber);
								_gcvt(tmpNumber, 5, calcedNode->str);
								Push(calcStack, calcedNode);
							}
							else {		// *的情况这种要报错
								return -1;
							}
						}
					}//两个都是2x这种情形
				}//----------两个都含有x的情形结尾-------------
				else {			//两个操作数中，只有一个未知数x时的计算
					if (ch == '+' || ch == '-') {		//+-的情况，直接将他们联合为一个字符串
						
						/*------------这里是因为，我要把格式统一为ax+b形式，所以要判断哪一个含有未知数x，那么它就在前面-------*/
						//这是普通情形，(2x) + 1, -1 - (2x),这种
						if (!complex_expression(calcNode1) && !complex_expression(calcNode2)) {
							simple_combine_singleX(calcNode1, calcNode2, calcStack, ch);
						}
						else {
							//这是复杂情形，(2x+1) - 1, (2x + 1) - -1 这种
							complex_combine_singleX1(calcNode1, calcNode2, calcStack, ch);
						}
					}
					else {			//*和/的情况，，如果是*，就简单的将系数和另一个操作数相乘即可，如果是/，判断含x的项是不是在前面，若不是，则这个式子就不是简单的一元一次方程，报错
						if (!complex_expression(calcNode1) && !complex_expression(calcNode2)) {		//没有复合表达式的话
							if (ch == '*') {		// *的情况
								if (calcNode1->flag == 1) {		//第一项为未知数项
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//把未知数项存到strTmp这个字符串里
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp1);
									double tmpNumber1 = atof(strTmp1);		//将其转为数字
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 * tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
								else {			//第二项为未知数
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//把未知数项存到strTmp这个字符串里
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp2);
									double tmpNumber1 = atof(strTmp1);		//将其转为数字
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 * tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
							}
							else {			// /的情况
								if (calcNode1->flag == 1) {   //未知项在前面，可以相除
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//把未知数项存到strTmp这个字符串里
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp1);
									double tmpNumber1 = atof(strTmp1);		//将其转为数字
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 / tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
								else {				//未知项在后面一位，报错，这不是一元一次
									return -1;
								}
							}
						}
						else {		// *和/有复杂表达式的情况
							if (complex_expression(calcNode1)) {
								complex_combine_singleX2(calcNode1, calcNode2, calcStack, ch);
							}
							else {
								if (ch == '/') {
									return -1;		//如果普通数字是被除数，那么就不是一元一次方程
								}
								complex_combine_singleX2(calcNode2, calcNode1, calcStack, ch);
							}
						}
					}
				}
			}
			else {			//不含未知数x的普通数字项
				List *calcedNode = (List *)malloc(sizeof(List));	//将计算完成的数字打包
				calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
				memset(calcedNode->str, '\0', strCapacity);
				calcedNode->flag = 0;
				calcedNode->next = NULL;
				calcedNode->priority = 0;
				simpleNumber_process(calcNode1->str, calcNode2->str, calcedNode->str,ch);		//调用函数来计算
				Push(calcStack, calcedNode);			//入栈
			}
			free(calcNode1->str);				//这里错误了一次，修复前查资料，可能原因：组数越界。 找到原因，之前有负数，存入节点时用sprintf函数，可能负数多了一个负号导致数组越界？以后考证，改用_gcvt函数。
			free(calcNode1);					//再次发生，确定就是写入数据的空间比malloc的空间大，产生溢出，就会报错
			free(calcNode2->str);
			free(calcNode2);
			pointer = pointer->next;
		}
	}
	return 0;
}

//两个ax+b式，合并为一个ax+b
int combineX(List *finalExpression1, List *finalExpression2, List *finalExpression) {
	if (finalExpression1->flag == 0) {		//左侧为普通数字
		if (finalExpression2->flag == 0) {
			return -1;
		}
		
		if (!complex_expression(finalExpression1) && !complex_expression(finalExpression2)) {
			simple_combine_singleX(finalExpression1, finalExpression2, finalExpression, '-');
		}
		else {
			//这是复杂情形，(2x+1) - 1, (2x + 1) - -1 这种
			complex_combine_singleX1(finalExpression1, finalExpression2, finalExpression, '-');
		}
	}
	else {		//左侧为未知数
		if (complex_expression(finalExpression1)) {		//左侧为2x+1这种
			if (finalExpression2->flag == 0) {		//右侧为常数
				complex_combine_singleX1(finalExpression1, finalExpression2, finalExpression, '-');
			}
			else {
				if (complex_expression(finalExpression2)) {
					double_complex_combine(finalExpression1, finalExpression2, finalExpression, '-');
				}
				else {
					complex_combine_doubleX(finalExpression1, finalExpression2, finalExpression, '-');
				}
			}
		}
		else {			//左侧为2x这种
			if (finalExpression2->flag == 0) {
				simple_combine_singleX(finalExpression1, finalExpression2, finalExpression, '-');
			}
			else {
				if (complex_expression(finalExpression2)) {
					double_complex_combine(finalExpression2, finalExpression1, finalExpression, '-');
				}
				else {		//两边都为2x 2x这种
					double tmpNumber = 0;								//用来存运算过后的数字
					char *strTmp1 = NULL;
					char *strTmp2 = NULL;
					malloc_calcMem(&strTmp1, &strTmp2);
					strcpy(strTmp1, finalExpression1->str);		//把未知数项存到strTmp这个字符串里
					strcpy(strTmp2, finalExpression2->str);
					remove_X(strTmp1);						//去掉了未知数项里面的x，剩下系数
					remove_X(strTmp2);
					double tmpNumber1 = atof(strTmp1);		//将其转为数字
					double tmpNumber2 = atof(strTmp2);
					tmpNumber = tmpNumber1 - tmpNumber2;

					List *calcedNode = (List *)malloc(sizeof(List));	//将计算完成的数字打包
					calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
					memset(calcedNode->str, '\0', strCapacity);
					calcedNode->flag = 1;
					calcedNode->next = NULL;
					calcedNode->priority = 0;
					//sprintf(calcedNode->str, "%lf", tmpNumber);
					_gcvt(tmpNumber, 5, calcedNode->str);
					char *addX = calcedNode->str;
					while (*addX != '\0') {
						addX++;
					}
					*addX = 'x';
					Push(finalExpression, calcedNode);			//入栈
					free(strTmp1);
					free(strTmp2);
				}
			}
		}
	}
	return 0;
}

double calculateX(List *expression) {
	char *coefficient = NULL;
	char *number = NULL;
	malloc_calcMem(&coefficient, &number);
	char *coefficientPtr = coefficient;
	char *numberPtr = number;
	char *strPtr = expression->str;

	while (*strPtr != 'x') {
		*coefficientPtr = *strPtr;
		coefficientPtr++;
		strPtr++;
	}
	strPtr++;		//现在指向操作符
	if (*strPtr == '+') {
		*numberPtr = '-';
		numberPtr++;
		strPtr++;
	}
	else {
		strPtr++;
	}
	while (*strPtr != '\0') {
		*numberPtr = *strPtr;
		numberPtr++;
		strPtr++;
	}
	double tmpCoefficient = atof(coefficient);		//将其转为数字
	double tmpNumber = atof(number);
	free(coefficient);
	free(number);
	double resualt = tmpNumber / tmpCoefficient;
	return resualt;
}

//此函数的作用是把表达式分为左右两个子表达式
int split(char *expression, char *sub_expression[2]) {
	if (expression == NULL) {
		return -1;
	}
	char *tmp = expression;
	char *tmp0 = sub_expression[0];
	char *tmp1 = sub_expression[1];
	while (*tmp != '=' && *tmp != '\0') {
		*tmp0 = *tmp;
		tmp++;
		tmp0++;
	}
	tmp0 = NULL;

	if (*tmp == '\0') {
		printf("错误，这不是一个等式\n");
		return -1;
	}
	tmp++;

	while (*tmp != '\0') {
		*tmp1 = *tmp;
		tmp++;
		tmp1++;
	}
	tmp1 = NULL;

	return 0;
}

//查看输入的表达式(测试函数)
void show_expression(char *expression) {
	printf("%s\n", expression);
}

//从链表中取出数字来显示(测试函数)
void show_sufixExpression(List *left) {
	if (left == NULL) {
		printf("没有后缀式\n");
		return;
	}
	List *tmp = left->next;
	printf("后缀表达式:");
	while (tmp != NULL) {
		printf("%s ", tmp->str);
		tmp = tmp->next;
	}
	printf("\n");
}
/*---------------------------------------------------------------------------*/

/*-----------------------------主过程----------------------------------------*/
int main(void) {
	int ret = 0;
	char *expression = (char *)malloc(200 * sizeof(char));		//原始表达式
	printf("请输入你的表达式：");
	gets_s(expression, 200);
	printf("%s\n", expression);

	char *sub_expression[2];
	sub_expression[0] = (char *)malloc(200 * sizeof(char));		//表达式左侧
	memset(sub_expression[0], '\0', 200);
	sub_expression[1] = (char *)malloc(200 * sizeof(char));		//表达式右侧
	memset(sub_expression[1], '\0', 200);

	ret = split(expression, sub_expression);
	if (ret == -1) {
		printf("表达式错误\n");
	}

	List *left = NULL;		//存表达式左侧的后缀表达式链
	List *right = NULL;		//存表达式右侧的后缀表达式链

	left = infix_TO_suffix(sub_expression[0]);
	right = infix_TO_suffix(sub_expression[1]);
	if (left == NULL || right == NULL) {
		printf("表达式输入错误\n");
		system("pause");
		return 0;
	}

	show_sufixExpression(left);
	show_sufixExpression(right);

	List *finalExpression1 = (List *)malloc(sizeof(List));
	finalExpression1->next = NULL;
	List *finalExpression2 = (List *)malloc(sizeof(List));
	finalExpression2->next = NULL;

	int finalRet1 = process(left, finalExpression1);
	int finalRet2 = process(right, finalExpression2);
	if (finalRet1 == -1 || finalRet2 == -1) {
		printf("在后缀表达式计算时出错，应该是表达式输入有错\n");
		system("pause");
		return 0;
	}
	List *finalExpression = (List *)malloc(sizeof(List));
	finalExpression->next = NULL;

	List *finalNode1 = NULL;
	List *finalNode2 = NULL;
	Pop(finalExpression1, &finalNode1);
	Pop(finalExpression2, &finalNode2);

	ret = combineX(finalNode1, finalNode2, finalExpression);
	if (ret == -1) {
		printf("在合并ax+b时发生错误\n");
		system("pause");
		return 0;
	}

	List *node = NULL;
	Pop(finalExpression, &node);
	double resualt = calculateX(node);
	printf("计算结果：x=%.4lf\n", resualt);
	system("pause");
	return 0;
}
//((1+2x)/3)-(7+(8-9))*(1/2)=5x+(3x-2)
//1 2x + 3 / 7 8 9 - + 1 2 / * -
//5x 3x 2 - +

//(2-5*3)/2=15+8-6/2

//((2+4)*4+3)/5+4=10-(20/4+5)/2

//5x+4+2*3=2x+3/2

//(5x+4)+(3x-1)+3=2x*5