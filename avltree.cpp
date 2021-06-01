#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <vector>

using namespace std;

#define mk make_pair

typedef struct AVLTree AVL;

struct AVLTree {
	int value;
	int key;
	AVL* left;
	AVL* right;
	int height;
};

AVL* root = NULL;

int getHeight(AVL* t);
void updateHeight(AVL* t);
AVL* singleRotateLeft(AVL* t);
AVL* singleRotateRight(AVL* t);
AVL* doubleRotateLeft(AVL* t);
AVL* doubleRotateRight(AVL* t);
AVL* insertAVL(int key, int x, AVL* t);
AVL* findAVL(int key, int x, AVL* t);
AVL* findMin(AVL* t);
AVL* deleteAVL(int key, int x, AVL* t);

vector<pair<int, int>> vec;

int main()
{
	FILE* fp;
	int n;
	char filename[20];
	char ch[101];
	char* str;
	int num, key;
	int cnt;
	while (1)
	{
		printf("������ ������ �Է��Ͻÿ�. (1. insertion, 2. deletion, 3. quit) : ");
		scanf("%d", &n);

		if (n == 1)
		{
			printf("insert�� ���ϸ��� �Է��Ͻÿ� : ");
			scanf("%s", filename);

			fp = fopen(filename, "r");

			for (int i = 0;i < 1000000;i++)
			{
				//input ���� ���� ���� �о�ͼ� tree�� ����
				fgets(ch, 100, fp);
				ch[strlen(ch) - 1] = '\0';
				str = strtok(ch, "\t");
				key = atoi(str);
				str = strtok(NULL, "\t");
				num = atoi(str);
				root = insertAVL(key, num, root);

				//input ���� �� ������ vector�� ���� - ���߿� �񱳸� ���� ���
				vec.push_back(mk(key, num));
			}

			fclose(fp);

			printf("input result ���� �� �Է� : ");
			scanf("%s", filename);

			fp = fopen(filename, "w");

			//tree�� ����� ���� input ���� ���� ���� ��
			cnt = 0;
			for (int i = 0;i < 1000000;i++)
			{
				if (findAVL(vec[i].first, vec[i].second, root) != NULL)
				{
					fprintf(fp, "%d\t%d\n", vec[i].first, vec[i].second);
					cnt++;
				}
			}
			if (cnt == 1000000)
				printf("input.csv�� input_result.csv�� ��ġ\n");

			fclose(fp);
		}
		else if (n == 2)
		{
			vec.clear();

			printf("delete�� ���ϸ��� �Է��Ͻÿ� : ");
			scanf("%s", filename);

			fp = fopen(filename, "r");

			int k = 500000;
			if (!strcmp(filename, "delete2.csv"))
				k = 499724;

			for (int i = 0;i < k;i++)
			{
				//delete ���� ���� ���� �о�ͼ� tree���� ����
				fgets(ch, 100, fp);
				ch[strlen(ch) - 1] = '\0';
				str = strtok(ch, "\t");
				key = atoi(str);
				str = strtok(NULL, "\t");
				num = atoi(str);
				root = deleteAVL(key, num, root);

				//delete ���� �� ������ vector�� ���� - ���߿� �񱳸� ���� ���
				vec.push_back(mk(key, num));
			}

			fclose(fp);

			//�Ʊ� delete�� 50������ ���� tree���� �ִ��� Ȯ���غ�
			cnt = 0;
			for (int i = 0;i < vec.size();i++)
			{
				AVL* tmp = findAVL(vec[i].first, vec[i].second, root);
				if (tmp != NULL)
					cnt++;
			}
			if (!cnt)
				printf("%d�� ��� ���� �Ϸ�\n", k);

			printf("delete result ���ϸ��� �Է��Ͻÿ� : ");
			scanf("%s", filename);

			fp = fopen(filename, "r");

			vec.clear();

			for (int i = 0;i < 1000000;i++)
			{
				//delete result ���� ���� ���� �о ������ vector�� ����
				fgets(ch, 100, fp);
				ch[strlen(ch) - 1] = '\0';
				str = strtok(ch, "\t");
				key = atoi(str);
				str = strtok(NULL, "\t");
				//N/A ���� value�� -1�� ����
				if (strcmp(str, "N/A"))
					num = atoi(str);
				else
					num = -1;

				vec.push_back(mk(key, num));
			}

			fclose(fp);

			printf("delete output ���� �� �Է� : ");
			scanf("%s", filename);

			fp = fopen(filename, "w");

			//delete result ���� ���� tree���� �����ϴ��� Ȯ��
			cnt = 0;
			for (int i = 0;i < vec.size();i++)
			{
				if (vec[i].second == -1)
				{
					fprintf(fp, "%d\tN/A\n", vec[i].first);
					continue;
				}
				fprintf(fp, "%d\t%d\n", vec[i].first, vec[i].second);
				if (findAVL(vec[i].first, vec[i].second, root) != NULL)
					cnt++;
			}
			if (cnt == 1000000 - k)
				printf("delete_result.csv�� delete_output.csv�� ��ġ\n");

			fclose(fp);
		}
		else
			break;
	}
}

int getHeight(AVL* t)
{
	if (t == NULL)
		return -1;
	return t->height;
}

void updateHeight(AVL* t)
{
	if (t == NULL)
		return;
	t->height = max(getHeight(t->left), getHeight(t->right)) + 1;
}

AVL* singleRotateLeft(AVL* t)
{
	AVL* tmp = t->left;
	t->left = tmp->right;
	tmp->right = t;

	updateHeight(t);
	updateHeight(tmp);
	return tmp;
}

AVL* singleRotateRight(AVL* t)
{
	AVL* tmp = t->right;
	t->right = tmp->left;
	tmp->left = t;

	updateHeight(t);
	updateHeight(tmp);
	return tmp;
}

AVL* doubleRotateLeft(AVL* t)
{
	t->left = singleRotateRight(t->left);
	return singleRotateLeft(t);
}

AVL* doubleRotateRight(AVL* t)
{
	t->right = singleRotateLeft(t->right);
	return singleRotateRight(t);
}

AVL* insertAVL(int key, int x, AVL* t)
{
	if (t == NULL)
	{
		t = (AVL*)malloc(sizeof(AVL));
		if (t == NULL)
		{
			printf("Error in insert %d\n", x);
			exit(1);
		}
		else
		{
			t->value = x;
			t->height = 0;
			t->left = t->right = NULL;
			t->key = key;
		}
	}
	else if (key < t->key)
	{
		t->left = insertAVL(key, x, t->left);
		if (getHeight(t->left) - getHeight(t->right) == 2)
		{
			if (key < t->left->key)
				t = singleRotateLeft(t);
			else
				t = doubleRotateLeft(t);
		}
	}
	else if (key > t->key)
	{
		t->right = insertAVL(key, x, t->right);
		if (getHeight(t->right) - getHeight(t->left) == 2)
		{
			if (key > t->right->key)
				t = singleRotateRight(t);
			else
				t = doubleRotateRight(t);
		}
	}

	updateHeight(t);
	return t;
}

AVL* findAVL(int key, int x, AVL* t)
{
	if (t == NULL)
		return NULL;
	if (t->key == key && t->value == x)
	{
		return t;
	}
	if (key < t->key)
		return findAVL(key, x, t->left);
	return findAVL(key, x, t->right);
}

AVL* findMin(AVL* t)
{
	if (t == NULL)
		return t;
	if (t->left == NULL)
		return t;
	return findMin(t->left);
}

AVL* deleteAVL(int key, int x, AVL* t)
{
	if (t == NULL)
	{
		printf("Error in delete %d\n", x);
		exit(1);
	}
	else if (key < t->key)
	{
		t->left = deleteAVL(key, x, t->left);
		if (getHeight(t->right) - getHeight(t->left) == 2)
		{
			if (getHeight(t->right->right) >= getHeight(t->right->left))
				t = singleRotateRight(t);
			else
				t = doubleRotateRight(t);
		}
	}
	else if (key > t->key)
	{
		t->right = deleteAVL(key, x, t->right);
		if (getHeight(t->left) - getHeight(t->right) == 2)
		{
			if (getHeight(t->left->left) >= getHeight(t->left->right))
				t = singleRotateLeft(t);
			else
				t = doubleRotateLeft(t);
		}
	}
	else if (t->left && t->right)
	{
		AVL* tmp = findMin(t->right);
		t->value = tmp->value;
		t->key = tmp->key;
		t->right = deleteAVL(t->key, t->value, t->right);
		if (getHeight(t->left) - getHeight(t->right) == 2)
		{
			if (getHeight(t->left->left) >= getHeight(t->left->right))
				t = singleRotateLeft(t);
			else
				t = doubleRotateLeft(t);
		}
	}
	else
	{
		AVL* tmp = t;
		if (t->left == NULL)
			t = t->right;
		else if (t->right == NULL)
			t = t->left;
		free(tmp);
	}

	updateHeight(t);
	return t;
}
