#include "mesh.h"

void main()
{
	setlocale(LC_ALL, "Russian");

	MESH disk;
	disk.input();

	int user_choise;
	bool end = false;
	while (!end) {
		printf("\t1) �� ������ �� �������� ����� ��� ������� �������;\n\t2) �� ���� ����� �������� ����� �����;\n");
		printf("\t3) �� ������ ����� ������ ������ ������, ������������ �����;\n");
		printf("\t4) �� ������ ����� ������ ����� �� ������ ����� �����������;\n\t5) �����;\n");
		printf("��� ����� : ");
		scanf("%d\n", &user_choise);
		switch (user_choise)
		{
		case 1:
			{
			int fe_id;
			printf("������� ����� �� : ");
			scanf("%d\n", &fe_id);
			vector <int> base_id;
			base_id = disk.returnTheBasicFuncOfFE(fe_id);
			printf("� %d - � �� ������ �������� ������� � �������� : \n\t\t", fe_id);
			for(int i =0;i<base_id.size();i++)
				printf("%d ",base_id[i]);
			}
			break;
		case 2:
			{
			printf("\t������� ����� ������ ������� : ");
			int A_id, B_id;
			scanf("%d\n", &A_id);
			printf("\t������� ����� ������ ������� : ");
			scanf("%d\n", &B_id);
			int edge_id;
			if(disk.returnTheNumberOfEdge(A_id,B_id,&edge_id)) 
				printf("\t������ ������� �������� ����� � ������� %d\n",edge_id);
			else
				printf("\t������ ���� ������ �� �������� �����\n");
			}
			break;
		case 3:
			{
			printf("\t������� ����� ����� : ");
			int edge_id,A_id, B_id;
			scanf("%d\n", &edge_id);
			if (disk.returnPointsFormingAnEdge(&A_id, &B_id, edge_id))
				printf("\t����� %d ������� ������� � �������� %d � %d\n", edge_id, A_id,B_id);
			else
				printf("\t�� ���������� ����� � ����� �������.\n");
			}
			break;
		case 4:
			{
			printf("\t������� ����� ����� : ");
			int edge_id;
			vector <int> FE;
			scanf("%d\n", &edge_id);
			disk.returnNumberFEIncludingEdge(edge_id);
			printf("����� � ������� %d ������ � �� � ��������: \n\t\t", edge_id);
			for (int i = 0; i < FE.size(); i++)
				printf("%d ", FE[i]);
		}
			break;
		case 5:
			end =! end ;
			break;
		default:
			printf("��� ����� �� ������� � ��������� ��������� ��������! ������� ����� �� 1 �� 5");
			break;
		}
	}
}