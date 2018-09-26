#include "mesh.h"

void main()
{
	setlocale(LC_ALL, "Russian");

	MESH disk;
	disk.input();

	int user_choise;
	bool end = false;
	while (!end) {
		printf("\t1) По номеру КЭ получить номер его базисых функций;\n\t2) По двум узлам получить номер ребра;\n");
		printf("\t3) По номеру ребра узнать номера вершин, составляющих ребро;\n");
		printf("\t4) По номеру ребра узнать каким КЭ данное ребро принадлежит;\n\t5) Выход;\n");
		printf("Ваш выбор : ");
		scanf("%d\n", &user_choise);
		switch (user_choise)
		{
		case 1:
			{
			int fe_id;
			printf("Введите номер КЭ : ");
			scanf("%d\n", &fe_id);
			vector <int> base_id;
			base_id = disk.returnTheBasicFuncOfFE(fe_id);
			printf("В %d - й КЭ входят базисные функции с номерами : \n\t\t", fe_id);
			for(int i =0;i<base_id.size();i++)
				printf("%d ",base_id[i]);
			}
			break;
		case 2:
			{
			printf("\tВведите номер первой вершины : ");
			int A_id, B_id;
			scanf("%d\n", &A_id);
			printf("\tВведите номер второй вершины : ");
			scanf("%d\n", &B_id);
			int edge_id;
			if(disk.returnTheNumberOfEdge(A_id,B_id,&edge_id)) 
				printf("\tДанные вершины образуют ребро с номером %d\n",edge_id);
			else
				printf("\tДанная пара вершин не образуют ребро\n");
			}
			break;
		case 3:
			{
			printf("\tВведите номер ребра : ");
			int edge_id,A_id, B_id;
			scanf("%d\n", &edge_id);
			if (disk.returnPointsFormingAnEdge(&A_id, &B_id, edge_id))
				printf("\tРебро %d обрауют вершины с номерами %d и %d\n", edge_id, A_id,B_id);
			else
				printf("\tНе существует ребра с таким номером.\n");
			}
			break;
		case 4:
			{
			printf("\tВведите номер ребра : ");
			int edge_id;
			vector <int> FE;
			scanf("%d\n", &edge_id);
			disk.returnNumberFEIncludingEdge(edge_id);
			printf("Ребро с номером %d входит в КЭ с номерами: \n\t\t", edge_id);
			for (int i = 0; i < FE.size(); i++)
				printf("%d ", FE[i]);
		}
			break;
		case 5:
			end =! end ;
			break;
		default:
			printf("Ваш выбор не выходит в множество возможных действий! Введите цифру от 1 до 5");
			break;
		}
	}
}