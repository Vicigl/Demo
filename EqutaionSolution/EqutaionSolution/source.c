#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strCapacity 20
/*����ʽ�������ڵ�*/
typedef struct LinkNode {
	int		flag;				//����ָʾ�ǲ���δ֪����,0Ϊ��ͨ�1Ϊδ֪����
	int		priority;			//���������ȼ�
	char	*str;				//���ʽ������
	struct LinkNode *next;
}List;

/*-----------------------------�����ջ�Ĳ���----------------------------------*/
//����ͷ�ڵ��ʼ��
void StackInit(List *stack) {
	stack = (List *)malloc(sizeof(List));
	stack->priority = 0;
	stack->flag = 0;
	stack->str = NULL;
	stack->next = NULL;
}

//��ջ
void Push(List *stack, List *node) {
	if (stack == NULL) {
		return;
	}
	node->next = stack->next;
	stack->next = node;
}

//��ջ
void Pop(List *stack, List **node) {
	if (stack->next == NULL) {
		return;
	}
	List *tmp = stack->next;
	stack->next = stack->next->next;
	tmp->next = NULL;
	*node = tmp;
}

//ɾ��ջ��Ԫ��
void Pop_delete(List *stack) {
	if (stack == NULL) {
		return;
	}
	stack->next = stack->next->next;
}

//β�巨
void node_Insert(List *stack, List *node) {
	if (stack == NULL) {
		return;
	}
	while (stack->next != NULL) {
		stack = stack->next;
	}
	stack->next = node;
	
}

//�õ�ջ��Ԫ�ص����ȼ�
int get_stack_top_priority(List *stack) {
	if (stack == NULL) {
		return -1;
	}
	return stack->next->priority;
}
/*-----------------------------------------------------------------------*/


/*------------------------------���ܺ���----------------------------------*/

//�ڵ���ڴ濽��������ע�⣬�ṹ������ָ�룬������Ҫ���
void nodememCopy(List *src, List **des) {
	List *local = (List *)malloc(sizeof(List));
	local->flag = src->flag;
	local->next = NULL;
	local->priority = src->priority;
	local->str = (char *)malloc(strCapacity * sizeof(char));		//���
	memset(local->str, '\0', strCapacity);
	strcpy(local->str, src->str);
	*des = local;
}

//ȥ���ַ�����β���Ǹ�x
void remove_X(char *des) {
	char *tmp = des;
	while (*tmp != 'x') {
		tmp++;
	}
	*tmp = '\0';
}

//�����ڴ�
void malloc_calcMem(char **strTmp1, char **strTmp2) {
	char *tmp1 = (char *)malloc(strCapacity * sizeof(char));
	memset(tmp1, '\0', strCapacity);
	char *tmp2 = (char *)malloc(strCapacity * sizeof(char));
	memset(tmp2, '\0', strCapacity);
	*strTmp1 = tmp1;
	*strTmp2 = tmp2;
}

//����ͨ���ִ�����ڵ�Ȼ����ջ
void dataPack_and_push(double tmpNumber, List *calcStack) {
	List *calcedNode = (List *)malloc(sizeof(List));	//��������ɵ����ִ��
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
	Push(calcStack, calcedNode);			//��ջ
}

//�ж����Ǹ��ϵı��ʽ������ͨ���ʽ������1+2x�������ǲ���ֱ�Ӵ���ģ�Ҫ�𿪿�
int complex_expression(List *node) {
	char *tmp = node->str;
	while (*tmp != '\0') {
		if (*tmp != '+' && *tmp != '-' && *tmp != '*' && *tmp != '/') {
			tmp++;
		}
		else {
			return 1;		//1�Ǹ��ӱ��ʽ
		}
	}
	return 0;		//0����ͨ����
}

//�ж�������ϱ��ʽ�Ĳ�������+����-,��+����1����-����0����û�оͷ���-1��������
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

//�ж�����������ǲ��Ǹ���,�Ǹ�������1�����Ƿ���0
int negative(List *node) {
	char *ptr = node->str;
	if (*ptr == '-') {
		return 1;
	}
	else {
		return 0;
	}
}


//�ж������ַ����ǲ�����ȵ�,Ϊ0����ȣ���Ϊ0�����
int equalStr(List *node1, List *node2) {
	int ret = strcmp(node1->str, node2->str);
	return ret;
}

//�Ѹ��ӱ��ʽ��ϵ������ͨ���ַ������
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

//��ͨ���������ֵĲ���
void simpleNumber_process(char *str1,char *str2, char *caledStr, char ch) {
	double tmpNumber1 = atof(str1);		//�õ�Ҫ���������������
	double tmpNumber2 = atof(str2);
	double tmpNumber = 0;							//������������������
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
	//sprintf(caledStr, "%lf", tmpNumber);  //��������ڴ渺��ʱ�ᷢ�����⣬����
	_gcvt(tmpNumber, 5, caledStr);		//�Ѹ�����ת��Ϊ�ַ���
	while (*caledStr != '\0') {			//��һ�ε������ǣ�����������Ǹ�λ����ʱ��д��caledStr�������ֺ����һ��'.' �����Ҫȥ�����'.'
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

//��������������ǰ�������ͨ���������Ϊһ���ַ���ax+b������1 + 2x, 1 - 2x, -1 - 2x,����������ͨδ֪��
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
	strcpy(strTmp1, nodeleft->str);		//��δ֪����浽strTmp����ַ�����
	strcpy(strTmp2, nodeRight->str);
	char *strTmp1Ptr = strTmp1;
	char *strTmp2Ptr = strTmp2;
	char *cpmplexStr = (char *)malloc(strCapacity * sizeof(char));
	memset(cpmplexStr, '\0', strCapacity);
	char *complexStrPointer = cpmplexStr;
	if (nodeleft->flag == 1) {			//δ֪����ǰ��
		if (negative(nodeRight)) {		//������Ϊ����
			while (*strTmp1Ptr != '\0') {				//�������ַ�������һ��
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
		else {			//������Ϊ����
			while (*strTmp1Ptr != '\0') {				//�������ַ�������һ��
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
	else {								//δ֪�����ں���,�м������Σ�(1+2x)  (1 + -2x) (-1+2x) (-1 + -2x) (1-2x) (1 - -2x) (-1 - 2x) (-1 - -2x)
		if (ch == '+') {
			while (*strTmp2Ptr != '\0') {
				*complexStrPointer = *strTmp2Ptr;
				complexStrPointer++;
				strTmp2Ptr++;
			}
			if (!negative(nodeleft)) {		//�������������������ŵ�����ʱ��Ҫ����ǰ���һ��+��
				*complexStrPointer = '+';
				complexStrPointer++;
			}
			while (*strTmp1Ptr != '\0') {				//�������ַ�������һ��
				*complexStrPointer = *strTmp1Ptr;
				complexStrPointer++;
				strTmp1Ptr++;
			}
		}
		else {		//��������-�����
			if (negative(nodeRight)) {   //ϵ������Ǹ���  (1 - -2x)  (-1 - -2x)
				strTmp2Ptr++;     //ȥ��ǰ���Ǹ����ţ���Ϊ��������
				while (*strTmp2Ptr != '\0') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				}
				if (!negative(nodeleft)) {
					*complexStrPointer = '+';
					complexStrPointer++;
				}
				while (*strTmp1Ptr != '\0') {				//�������ַ�������һ��
					*complexStrPointer = *strTmp1Ptr;
					complexStrPointer++;
					strTmp1Ptr++;
				}
			}
			else {     //ϵ�����������  (1-2x)  (-1 - 2x)
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
				while (*strTmp1Ptr != '\0') {				//�������ַ�������һ��
					*complexStrPointer = *strTmp1Ptr;
					complexStrPointer++;
					strTmp1Ptr++;
				}
			}
		}
	}
	calcedNode->str = cpmplexStr;
	Push(calcStack, calcedNode);			//��ջ
	free(strTmp1);
	free(strTmp2);
}

//��������������ǰ��������������Ϊһ���ַ���(+-�����)������һ���Ǹ������ַ���������2x+1,-2x+1,��һ������ͨ���֣�����1,2,-3
//��������ž���������ʽ��(2x+1) - 1 , (2x-3) + -4
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
	strcpy(strTmp1, nodeleft->str);		//��δ֪����浽strTmp����ַ�����
	strcpy(strTmp2, nodeRight->str);
	char *cpmplexStr = (char *)malloc(strCapacity * sizeof(char));
	memset(cpmplexStr, '\0', strCapacity);
	char *complexStrPointer = cpmplexStr;
	if (complex_expression(nodeleft)) {		//����Ǹ��ӱ��ʽ
		char *str1 = NULL;
		char *str2 = NULL;
		malloc_calcMem(&str1, &str2);
		char *str1Ptr = str1;
		char *str2Ptr = str2;

		while (*strTmp1Ptr != '+' && *strTmp1Ptr != '-') {
			*complexStrPointer = *strTmp1Ptr;
			complexStrPointer++;
			strTmp1Ptr++;
		} //����tempָ��ָ�������������,complexStrPointerָ����һ��Ҫ�����ַ�����
		if (judge_plus(nodeleft)) {
			strTmp1Ptr++;
		}
		while (*strTmp1Ptr != '\0') {
			*str1Ptr = *strTmp1Ptr;
			str1Ptr++;
			strTmp1Ptr++;
		}
		while (*strTmp2Ptr != '\0') {		//������,strTemp2ָ�������ͨ�����ַ���
			*str2Ptr = *strTmp2Ptr;
			str2Ptr++;
			strTmp2Ptr++;
		}
		char *caledStr = (char *)malloc(strCapacity * sizeof(char));
		memset(caledStr, '\0', strCapacity);
		char *caledStrPtr = caledStr;
		simpleNumber_process(str1, str2, caledStr, ch);			//��(2x+1) - -1 �е�1 - -1�������������caledStr����
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
		free(caledStr);		//�ǵð�����ڴ��ͷŵ�
	}
	else {		//�Ҳ��Ǹ��ӱ��ʽ
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
			} //����tempָ��ָ�������������,complexStrPointerָ����һ��Ҫ�����ַ�����
		}
		else {		//ch == '-'
			if (*strTmp2Ptr == '-') {
				strTmp2Ptr++;
				while (*strTmp2Ptr != '+' && *strTmp2Ptr != '-') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				} //����tempָ��ָ�������������,complexStrPointerָ����һ��Ҫ�����ַ�����
			}
			else {
				*complexStrPointer = '-';
				while (*strTmp2Ptr != '+' && *strTmp2Ptr != '-') {
					*complexStrPointer = *strTmp2Ptr;
					complexStrPointer++;
					strTmp2Ptr++;
				} //����tempָ��ָ�������������,complexStrPointerָ����һ��Ҫ�����ַ�����
			}
		}

		while (*strTmp1Ptr != '\0') {		//���ǰ�����Ǹ���ͨ����ȡ����
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
		simpleNumber_process(str1, str2, caledStr, ch);			//��(2x+1) - -1 �е�1 - -1�������������caledStr����
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
		free(caledStr);		//�ǵð�����ڴ��ͷŵ�
	}
	calcedNode->str = cpmplexStr;
	Push(calcStack, calcedNode);			//��ջ
	free(strTmp1);
	free(strTmp2);
}

//��������������ǰ��������������Ϊһ���ַ�����*/�������������һ���Ǹ������ַ���������2x+1,-2x+1,��һ������ͨ���֣�����1,2,-3
//��������ž���������ʽ��(2x+1) * 1 , (2x-3) / (-4)
void complex_combine_singleX2(List *nodeleft, List *nodeRight, List *calcStack, char ch) {
	char *node1strTmp1 = NULL;				//������2x+1��ϵ��2�ͳ���1���ַ��������α�ָ��
	char *node1strTmp2 = NULL;
	malloc_calcMem(&node1strTmp1, &node1strTmp2);
	char *node1StrPtr1 = node1strTmp1;
	char *node1StrPtr2 = node1strTmp2;

	char *node2strTmp1 = NULL;				//������(2x+1)*(-5)��5����-5���ַ��������α�ָ��
	char *node2strTmp2 = NULL;
	malloc_calcMem(&node2strTmp1, &node2strTmp2);
	char *node2StrPtr1 = node2strTmp1;
	char *node2StrPtr2 = node2strTmp2;

	char *node1Ptr = nodeleft->str;		//(2x+1)��(-5)��ԭʼ�ַ���ָ��
	char *node2Ptr = nodeRight->str;

	while (*node1Ptr != 'x') {
		*node1StrPtr1 = *node1Ptr;
		node1StrPtr1++;
		node1Ptr++;
	}
	node1Ptr++;		//����x������ָ���ӱ��ʽ�Ĳ�������
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
	//----------------����ȡϵ���ͳ����ַ����Ĺ����Ѿ�����,�������ü�����------
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

//��������������ǰ��������������Ϊһ���ַ���,�������Ǻ�x��δ֪�һ���Ǹ����һ������ͨx���ž���(2x-1) + 2x,2x - (2x-1)
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

//��������������ǰ��������������Ϊһ���ַ���,�������Ǻ�x��δ֪�һ���Ǹ����һ������ͨx���ž���(2x-1) + (2x+5),(5x-3) - (2x-1)
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

//��׺���ʽת��׺���ʽ
List *infix_TO_suffix(char *expression) {
	if (expression == NULL) {
		return NULL;
	}

	List *element = (List *)malloc(sizeof(List));
	element->flag = 0;
	element->next = NULL;
	element->priority = 0;
	element->str = NULL;

	char *temp = expression;		//����ָ��

	List *stack = NULL;				//��ʱ���Ǹ�������� ()+-*/ ��ջ
	stack = (List *)malloc(sizeof(List));
	stack->priority = 0;
	stack->next = NULL;
	stack->str = NULL;

	List *first_Node = (List *)malloc(sizeof(List));		//��һ�����ȼ�Ϊ0�Ľڵ�
	first_Node->flag = 0;
	first_Node->next = NULL;
	first_Node->str = NULL;
	first_Node->priority = 0;
	Push(stack, first_Node);		//���ȼ�Ϊ0������ڵ���ջ

	while (*temp != '\0') {			//ѭ�������ַ������ÿ���ַ�
		if (*temp == 'x') {			//��������ǵ�����Ϊx������
			char *singleX = (char *)malloc(strCapacity * sizeof(char));
			memset(singleX, '\0', strCapacity);
			char *singleXPtr = singleX;
			*singleXPtr++ = '1';		//��������Ҫ��Ϊ�˼��㷽���������ϵ��Ϊ1��xǰ�涼����һ��1
			*singleXPtr = *temp;
			List *node = (List *)malloc(sizeof(List));
			node->str = singleX;
			node->flag = 1;
			node->priority = 0;
			node->next = NULL;
			node_Insert(element, node);		//��ͨ�������
			temp++;
		}
		else if (*temp == '+' || *temp == '-' || *temp == '*' || *temp == '/' || *temp == '(' || *temp == ')') {		//����Ϊ��ͨ��+-*/()
			int out_priority;								//+-*/��ջ������ȼ�

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

			int in_priority = get_stack_top_priority(stack);	//ȡ����ʱջ�������ȼ�
			if (out_priority > in_priority) {					//һ��ջ��>ջ�ڣ���ջ
				Push(stack, node);		//��ջ
			}
			else if (out_priority < in_priority) {				//����ջ��<ջ��,��ջ�����,���Ұ����ȼ��ߵ��Ǹ���������ջ
				while (out_priority < get_stack_top_priority(stack)) {		//���ջ��Ԫ�����ȼ�һֱ�������Ǹ�Ԫ�ش�ѭ��������ջ���
					List *tmpnode = NULL;
					Pop(stack, &tmpnode);		//��ջ
					tmpnode->next = NULL;
					node_Insert(element, tmpnode);		//���
				}

				if (*(node->str) != ')') {		//������ǡ�)�����ţ��ͽ�ջ����ָ')'û�б�Ҫ��ջ
					Push(stack, node);
				}
				else {							//�����')'���ţ��������ý�ջ�������ж�ջ���ǲ���'('���ţ����ǣ��������һ�ԣ�û�ã���ջ������'('
					if (*(stack->next->str) == '(') {
						Pop_delete(stack);
					}
				}
			}
			else {												//����ջ��=ջ�ڣ�ֱ����ջ��ֻ�У��������㣬
				Pop_delete(stack);
			}
			temp++;
		}
		else if (*temp >= 48 && *temp <= 57) {
			char *s = (char *)malloc(strCapacity * sizeof(char));
			memset(s, '\0', strCapacity);
			List *node = (List *)malloc(sizeof(List));

			char *tmp = s;				//���Ǵ���������α�
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
			node_Insert(element, node);		//��ͨ�������
		}
		else {			//���ڷ���ѧ���ʽ����
			return NULL;	//���ʽ����
		}
	}

	while (stack->next->str != NULL) {		//��ջ��ʣ��Ĳ��������
		List *tmpnode = NULL;
		Pop(stack, &tmpnode);
		tmpnode->next = NULL;
		if (*(tmpnode->str) != '(' && *(tmpnode->str) != ')') {
			node_Insert(element, tmpnode);
		}
	}

	return element;
}

//�����������������׺���ʽת��Ϊax+b��ʽ��
int process(List *suffix_expression, List *calcStack) {
	List *pointer = suffix_expression->next;  //��׺���ʽ����һ���α�
	while (pointer != NULL) {
		if (*(pointer->str) != '+' && *(pointer->str) != '-' && *(pointer->str) != '*' && *(pointer->str) != '/') {		//������ͨ���������������������ջ
			List *node = NULL;
			nodememCopy(pointer, &node);
			Push(calcStack, node);
			pointer = pointer->next;
		}
		else {			//���־�������������ˣ���Ҫ�ȴ�ջ�е������������м���
			List *calcNode1 = NULL;
			List *calcNode2 = NULL;
			Pop(calcStack, &calcNode2);			//����ע�⣬calcNode2���ڲ����������Ǹ�����������ջ��������˵Ҫ��ȡ������ȡcalcNode1
			Pop(calcStack, &calcNode1);
			char ch = *(pointer->str);									//���ǲ�����
			if (calcNode1->flag == 1 || calcNode2->flag == 1) {			//����x��δ֪����
				if (calcNode1->flag == 1 && calcNode2->flag == 1) {		//��ӵ��������Ǻ���x��δ֪����
					int ret1 = complex_expression(calcNode1);
					int ret2 = complex_expression(calcNode2);
					if (ret1 == 1 && ret2 == 1) {			//��ߺ��ұ߶��Ǹ��ӱ��ʽ��ֻ��ʵ��+-/������/������£�����δ֪�����һ��
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
						else {     //������(1+2x)*(1+2x)�������Σ���
							return -1;
						}
					}
					else if ((ret1 == 1 && ret2 == 0) || (ret1 == 0 && ret2 == 1)) {		//һ�����ӣ�һ���򵥣�ֻ��+-
						if (ret1 == 1 && ret2 == 0) {		//��߸��ӣ��ұ߼�
							if (ch == '*' || ch == '/') {
								return -1;
							}
							complex_combine_doubleX(calcNode1, calcNode2, calcStack, ch);
						}
						else {			//�ұ߸��ӣ���߼�
							if (ch == '*' || ch == '/') {
								return -1;
							}
							complex_combine_doubleX(calcNode2, calcNode1, calcStack, ch);
						}
					}
					else {		//�������Ǽ�δ֪����
						double tmpNumber = 0;								//������������������
						char *strTmp1 = NULL;
						char *strTmp2 = NULL;
						malloc_calcMem(&strTmp1, &strTmp2);
						strcpy(strTmp1, calcNode1->str);		//��δ֪����浽strTmp����ַ�����
						strcpy(strTmp2, calcNode2->str);
						remove_X(strTmp1);						//ȥ����δ֪���������x��ʣ��ϵ��
						remove_X(strTmp2);
						double tmpNumber1 = atof(strTmp1);		//����תΪ����
						double tmpNumber2 = atof(strTmp2);
						switch (ch)								//���ݲ�������ͬѡ��ͬ�Ĳ�����ʽ
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
						if (ch == '+' || ch == '-') {		//+-�������������������x������tmpNumberֻ������ϵ��������tmpNumber=0���Զ�����һ���ָ��Ҫ��ĩβ��һ��x
							if (tmpNumber != 0) {
								List *calcedNode = (List *)malloc(sizeof(List));	//��������ɵ����ִ��
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
								Push(calcStack, calcedNode);			//��ջ
							}
						}
						else {								//*��/�������/���������x�������ˣ���ֻ��ʣ��һ���������*��������η������������⣬����
							if (ch == '/') {
								List *calcedNode = (List *)malloc(sizeof(List));	//��������ɵ����ִ��
								calcedNode->str = (char *)malloc(sizeof(char));
								memset(calcedNode->str, '\0', strCapacity);
								calcedNode->flag = 0;
								calcedNode->next = NULL;
								calcedNode->priority = 0;
								//sprintf(calcedNode->str, "%lf", tmpNumber);
								_gcvt(tmpNumber, 5, calcedNode->str);
								Push(calcStack, calcedNode);
							}
							else {		// *���������Ҫ����
								return -1;
							}
						}
					}//��������2x��������
				}//----------����������x�����ν�β-------------
				else {			//�����������У�ֻ��һ��δ֪��xʱ�ļ���
					if (ch == '+' || ch == '-') {		//+-�������ֱ�ӽ���������Ϊһ���ַ���
						
						/*------------��������Ϊ����Ҫ�Ѹ�ʽͳһΪax+b��ʽ������Ҫ�ж���һ������δ֪��x����ô������ǰ��-------*/
						//������ͨ���Σ�(2x) + 1, -1 - (2x),����
						if (!complex_expression(calcNode1) && !complex_expression(calcNode2)) {
							simple_combine_singleX(calcNode1, calcNode2, calcStack, ch);
						}
						else {
							//���Ǹ������Σ�(2x+1) - 1, (2x + 1) - -1 ����
							complex_combine_singleX1(calcNode1, calcNode2, calcStack, ch);
						}
					}
					else {			//*��/��������������*���ͼ򵥵Ľ�ϵ������һ����������˼��ɣ������/���жϺ�x�����ǲ�����ǰ�棬�����ǣ������ʽ�ӾͲ��Ǽ򵥵�һԪһ�η��̣�����
						if (!complex_expression(calcNode1) && !complex_expression(calcNode2)) {		//û�и��ϱ��ʽ�Ļ�
							if (ch == '*') {		// *�����
								if (calcNode1->flag == 1) {		//��һ��Ϊδ֪����
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//��δ֪����浽strTmp����ַ�����
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp1);
									double tmpNumber1 = atof(strTmp1);		//����תΪ����
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 * tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
								else {			//�ڶ���Ϊδ֪��
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//��δ֪����浽strTmp����ַ�����
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp2);
									double tmpNumber1 = atof(strTmp1);		//����תΪ����
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 * tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
							}
							else {			// /�����
								if (calcNode1->flag == 1) {   //δ֪����ǰ�棬�������
									char *strTmp1 = NULL;
									char *strTmp2 = NULL;
									malloc_calcMem(&strTmp1, &strTmp2);
									strcpy(strTmp1, calcNode1->str);		//��δ֪����浽strTmp����ַ�����
									strcpy(strTmp2, calcNode2->str);
									remove_X(strTmp1);
									double tmpNumber1 = atof(strTmp1);		//����תΪ����
									double tmpNumber2 = atof(strTmp2);
									double tmpNumber = tmpNumber1 / tmpNumber2;

									dataPack_and_push(tmpNumber, calcStack);
								}
								else {				//δ֪���ں���һλ�������ⲻ��һԪһ��
									return -1;
								}
							}
						}
						else {		// *��/�и��ӱ��ʽ�����
							if (complex_expression(calcNode1)) {
								complex_combine_singleX2(calcNode1, calcNode2, calcStack, ch);
							}
							else {
								if (ch == '/') {
									return -1;		//�����ͨ�����Ǳ���������ô�Ͳ���һԪһ�η���
								}
								complex_combine_singleX2(calcNode2, calcNode1, calcStack, ch);
							}
						}
					}
				}
			}
			else {			//����δ֪��x����ͨ������
				List *calcedNode = (List *)malloc(sizeof(List));	//��������ɵ����ִ��
				calcedNode->str = (char *)malloc(strCapacity * sizeof(char));
				memset(calcedNode->str, '\0', strCapacity);
				calcedNode->flag = 0;
				calcedNode->next = NULL;
				calcedNode->priority = 0;
				simpleNumber_process(calcNode1->str, calcNode2->str, calcedNode->str,ch);		//���ú���������
				Push(calcStack, calcedNode);			//��ջ
			}
			free(calcNode1->str);				//���������һ�Σ��޸�ǰ�����ϣ�����ԭ������Խ�硣 �ҵ�ԭ��֮ǰ�и���������ڵ�ʱ��sprintf���������ܸ�������һ�����ŵ�������Խ�磿�Ժ�֤������_gcvt������
			free(calcNode1);					//�ٴη�����ȷ������д�����ݵĿռ��malloc�Ŀռ�󣬲���������ͻᱨ��
			free(calcNode2->str);
			free(calcNode2);
			pointer = pointer->next;
		}
	}
	return 0;
}

//����ax+bʽ���ϲ�Ϊһ��ax+b
int combineX(List *finalExpression1, List *finalExpression2, List *finalExpression) {
	if (finalExpression1->flag == 0) {		//���Ϊ��ͨ����
		if (finalExpression2->flag == 0) {
			return -1;
		}
		
		if (!complex_expression(finalExpression1) && !complex_expression(finalExpression2)) {
			simple_combine_singleX(finalExpression1, finalExpression2, finalExpression, '-');
		}
		else {
			//���Ǹ������Σ�(2x+1) - 1, (2x + 1) - -1 ����
			complex_combine_singleX1(finalExpression1, finalExpression2, finalExpression, '-');
		}
	}
	else {		//���Ϊδ֪��
		if (complex_expression(finalExpression1)) {		//���Ϊ2x+1����
			if (finalExpression2->flag == 0) {		//�Ҳ�Ϊ����
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
		else {			//���Ϊ2x����
			if (finalExpression2->flag == 0) {
				simple_combine_singleX(finalExpression1, finalExpression2, finalExpression, '-');
			}
			else {
				if (complex_expression(finalExpression2)) {
					double_complex_combine(finalExpression2, finalExpression1, finalExpression, '-');
				}
				else {		//���߶�Ϊ2x 2x����
					double tmpNumber = 0;								//������������������
					char *strTmp1 = NULL;
					char *strTmp2 = NULL;
					malloc_calcMem(&strTmp1, &strTmp2);
					strcpy(strTmp1, finalExpression1->str);		//��δ֪����浽strTmp����ַ�����
					strcpy(strTmp2, finalExpression2->str);
					remove_X(strTmp1);						//ȥ����δ֪���������x��ʣ��ϵ��
					remove_X(strTmp2);
					double tmpNumber1 = atof(strTmp1);		//����תΪ����
					double tmpNumber2 = atof(strTmp2);
					tmpNumber = tmpNumber1 - tmpNumber2;

					List *calcedNode = (List *)malloc(sizeof(List));	//��������ɵ����ִ��
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
					Push(finalExpression, calcedNode);			//��ջ
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
	strPtr++;		//����ָ�������
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
	double tmpCoefficient = atof(coefficient);		//����תΪ����
	double tmpNumber = atof(number);
	free(coefficient);
	free(number);
	double resualt = tmpNumber / tmpCoefficient;
	return resualt;
}

//�˺����������ǰѱ��ʽ��Ϊ���������ӱ��ʽ
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
		printf("�����ⲻ��һ����ʽ\n");
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

//�鿴����ı��ʽ(���Ժ���)
void show_expression(char *expression) {
	printf("%s\n", expression);
}

//��������ȡ����������ʾ(���Ժ���)
void show_sufixExpression(List *left) {
	if (left == NULL) {
		printf("û�к�׺ʽ\n");
		return;
	}
	List *tmp = left->next;
	printf("��׺���ʽ:");
	while (tmp != NULL) {
		printf("%s ", tmp->str);
		tmp = tmp->next;
	}
	printf("\n");
}
/*---------------------------------------------------------------------------*/

/*-----------------------------������----------------------------------------*/
int main(void) {
	int ret = 0;
	char *expression = (char *)malloc(200 * sizeof(char));		//ԭʼ���ʽ
	printf("��������ı��ʽ��");
	gets_s(expression, 200);
	printf("%s\n", expression);

	char *sub_expression[2];
	sub_expression[0] = (char *)malloc(200 * sizeof(char));		//���ʽ���
	memset(sub_expression[0], '\0', 200);
	sub_expression[1] = (char *)malloc(200 * sizeof(char));		//���ʽ�Ҳ�
	memset(sub_expression[1], '\0', 200);

	ret = split(expression, sub_expression);
	if (ret == -1) {
		printf("���ʽ����\n");
	}

	List *left = NULL;		//����ʽ���ĺ�׺���ʽ��
	List *right = NULL;		//����ʽ�Ҳ�ĺ�׺���ʽ��

	left = infix_TO_suffix(sub_expression[0]);
	right = infix_TO_suffix(sub_expression[1]);
	if (left == NULL || right == NULL) {
		printf("���ʽ�������\n");
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
		printf("�ں�׺���ʽ����ʱ����Ӧ���Ǳ��ʽ�����д�\n");
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
		printf("�ںϲ�ax+bʱ��������\n");
		system("pause");
		return 0;
	}

	List *node = NULL;
	Pop(finalExpression, &node);
	double resualt = calculateX(node);
	printf("��������x=%.4lf\n", resualt);
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